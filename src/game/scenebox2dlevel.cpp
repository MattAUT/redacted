// COMP710 GP Framework 2022// COMP710 GP Framework 2022

#include "scenebox2dlevel.h"

#include <engine/renderer.h>
#include <engine/sprite.h>
#include <engine/scenemanager.h>
#include <engine/soundmanager.h>
#include <engine/xinputcontroller.h>
#include <engine/camera.h>
#include <engine/camera.h>
#include <logmanager.h>
#include <utils.h>
#include <ini.h>

#include "player.h"
#include "enemy.h"
#include "collectable.h"
#include "enemy.h"
#include "safearea.h"
#include "tile.h"
#include "tilecontainer.h"
#include "hud.h"
#include "collisionCreation.h"
#include "currentLevelLoaded.h"
#include "pausescreen.h"

#include <cmath>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <box2d/box2d.h>
#include <point.h>
#include <segment.h>
#include <polygon.h>
#include <fmod.hpp>

#include "SDL.h"
#include "SDL_opengl.h"

const char wallSafeZone = 's';
const char groundSafeZone = 'm';

const char safeZoneCenter = 'u';
const char wallLevelOne = 'a';
const char groundLevelOne = 'b';
const char wallLevelTwo = 'c';
const char groundLevelTwo = 'd';
const char wallLevelThree = 'e';
const char groundLevelThree = 'f';
const char itemOne = 'i';
const char enemy = 'k';
const char player = 'p';

const char loadLevel1 = '7';
const char loadLevel2 = '8';
const char loadLevel3 = '9';

#ifdef _DEBUG
#include <imgui/imgui_impl_sdl.h>
#endif // _DEBUG

SceneBox2DLevel::SceneBox2DLevel() :
	currentLevel(0)
{
#ifdef _DEBUG
	dm_name = "Menu Scene";
#endif // _DEBUG
}

SceneBox2DLevel::~SceneBox2DLevel()
{
	delete m_pPlayer;
	m_pPlayer = 0;

	delete safeArea;
	safeArea = 0;

	for (Collectable* collectable : WorldItems)
	{
		//delete collectable;
	}
	WorldItems.clear();

	for (Sprite* sprite : WorldTiles)
	{
		delete sprite;
	}
	WorldTiles.clear();

	delete tileContainer;
	tileContainer = 0;

	delete safeAreaTileContainer;
	safeAreaTileContainer = 0;

	delete gameHUD;
	gameHUD = 0;

	delete m_pPauseScreen;
	m_pPauseScreen = 0;
}

bool SceneBox2DLevel::Initialise(Renderer& renderer, Camera& camera)
{
	m_pWorld = SceneManager::GetInstance().m_pWorld;

	m_pPlayer = new Player;
	m_pPlayer->Initialise(renderer);

	safeArea = new SafeArea;

	m_pPauseScreen = new PauseScreen;
	m_pPauseScreen->Initialise(renderer);

	LoadItems(renderer);
	LoadLevel(renderer);
	LoadEnemies(renderer);
	LoadWallCollision(renderer);
	LoadNavMesh();

	m_pPlayer->Setup(m_safeZoneCenter);

	gameHUD = new HUD;
	gameHUD->Initialise(renderer, m_pPlayer, safeArea);

	return Scene::Initialise(renderer, camera);
}

void SceneBox2DLevel::Process(float deltaTime, Input& input)
{
	m_pPauseScreen->Process(deltaTime, input);
	if (m_pPauseScreen->GetIsPaused())
	{
		return;
	}

	m_pPlayer->Process(deltaTime, input);
	m_pCamera->SetPosition(m_pPlayer->GetPosition());
	gameHUD->Process(deltaTime, input);

	if (levelOneLoad->GetPlayerInArea() == true) { currentLevel = 1; }
	if (levelTwoLoad->GetPlayerInArea() == true) { currentLevel = 2; }
	if (levelThreeLoad->GetPlayerInArea() == true) { currentLevel = 3; }

	for (unsigned int num = 0; num < WorldEnemies.size(); num++)
	{
		WorldEnemies[num]->Process(deltaTime, input);
	}


	for (unsigned int num = 0; num < WorldItems.size(); num++)
	{
		WorldItems[num]->Process(deltaTime, input);

		//Destroyes the box2d body after player collection to turn off collision.
		if (WorldItems[num]->GetIsToBeDeleted())
		{
			WorldItems[num]->GetBody()->SetEnabled(false);
			WorldItems[num]->SetIsToBeDeleted(false);
		}
	}

	for (unsigned int num = 0; num < WorldEnemies.size(); num++)
	{
		WorldEnemies[num]->Process(deltaTime, input);
	}
}

void SceneBox2DLevel::Draw(Renderer& renderer)
{
	if (currentLevel == 1) { tileContainer->Draw(renderer); }
	else if (currentLevel == 2) { tileContainer1->Draw(renderer); }
	else if (currentLevel == 3) { tileContainer2->Draw(renderer); }

	safeAreaTileContainer->Draw(renderer);

	for (unsigned int num = 0; num < WorldItems.size(); num++)
	{
		WorldItems[num]->Draw(renderer);
	}

	for (unsigned int num = 0; num < WorldEnemies.size(); num++)
	{
		WorldEnemies[num]->Draw(renderer);
	}

	safeArea->Draw(renderer);
	m_pPlayer->Draw(renderer);

	if (isLevelComplete)
	{
		// handle win state
	}

	gameHUD->Draw(renderer);


	m_pPauseScreen->Draw(renderer);
}

void
SceneBox2DLevel::Reset()
{

}

#ifdef _DEBUG
void SceneBox2DLevel::DebugDraw(Renderer& renderer)
{
	m_pPlayer->DebugDraw();
	//m_pCollectable->DebugDraw();

	safeArea->DebugDraw();
	gameHUD->DebugDraw();
	m_pPauseScreen->DebugDraw();

	ImGui::Begin("Enemies");
	for (unsigned int num = 0; num < WorldEnemies.size(); num++)
	{
		WorldEnemies[num]->DebugDraw();
	}
	ImGui::End();
}
#endif // _DEBUG

void SceneBox2DLevel::SetCurrentLevel(int level) {
	currentLevel = level;
}

void
SceneBox2DLevel::LoadLevel(Renderer& renderer)
{
	tileContainer = new TileContainer;
	tileContainer1 = new TileContainer;
	tileContainer2 = new TileContainer;
	safeAreaTileContainer = new TileContainer;

	//safeZone = new TileContainer;

	std::ifstream levelTileDataPos;
	std::string line;

	levelTileDataPos.open("assets\\levels\\tiles.loc");

	//Creates a variable for the current row being iterated through
	int currentRow = 0;

	//Go through the text files contents saving each character to use as a tile determiner for the map tiles
	b2BodyDef bodyDef2;
	b2PolygonShape groundShape;
	b2FixtureDef fixtureDef2;

	m_pPlayer->SetPosition(Vector2((14), (-2)));

	while (std::getline(levelTileDataPos, line))
	{
		for (std::string::size_type i = 0; i < line.size(); i++)
		{
			//Initializes a new sprite
			mapTiles = new Sprite;

			// Initializes a new vector2 for the sprites location and saves it into a vector
			Tile* tile = new Tile;

			//Checks what type of tile should be generated for the above sprite
			//Walls
			if (line[i] == wallLevelOne || line[i] == wallLevelTwo || line[i] == wallLevelThree || line[i] == wallSafeZone)
			{
				mapTiles = renderer.CreateSprite("assets\\sprites\\level\\border.png", SpriteOrigin::CENTER);
				WorldTiles.push_back(mapTiles);

				tile->Initialise(mapTiles, i, currentRow);
				UserData* wallData = new UserData;
				wallData->m_type = ET_WALL;

				if (line[i] == wallLevelOne) { tileContainer->AddTile(tile); }
				else if (line[i] == wallLevelTwo) { tileContainer1->AddTile(tile); }
				else if (line[i] == wallLevelThree) { tileContainer2->AddTile(tile); }
				else if (line[i] == wallSafeZone) { safeAreaTileContainer->AddTile(tile); }

				//Creates a  
				bodyDef2.position.Set((i), (currentRow));
				bodyDef2.userData.pointer = (uintptr_t)wallData;
				m_testGroundBody = m_pWorld->CreateBody(&bodyDef2);

				//Fixture def
				groundShape.SetAsBox(0.5, 0.5);

				/*fixtureDef2.shape = &groundShape;
				fixtureDef2.friction = 0.3f;
				fixtureDef2.userData.pointer = (uintptr_t)this;
				fixtureDef2.filter.categoryBits = CATEGORY_WALLS;
				fixtureDef2.filter.maskBits = MASK_WALLS;
				m_pTestFixture = m_testGroundBody->CreateFixture(&fixtureDef2);*/
			}
			//Ground
			else if (line[i] == groundLevelOne || line[i] == groundLevelTwo || line[i] == groundLevelThree || line[i] == safeZoneCenter || line[i] == groundSafeZone)
			{
				mapTiles = renderer.CreateSprite("assets\\sprites\\level\\open.png", SpriteOrigin::CENTER);
				WorldTiles.push_back(mapTiles);

				tile->Initialise(mapTiles, i, currentRow);

				if (line[i] == groundLevelOne) { tileContainer->AddTile(tile); }
				else if (line[i] == groundLevelTwo) { tileContainer1->AddTile(tile); }
				else if (line[i] == groundLevelThree) { tileContainer2->AddTile(tile); }
				else if (line[i] == groundSafeZone) { safeAreaTileContainer->AddTile(tile); }
				else if (line[i] == safeZoneCenter)
				{
					safeArea->Initialise(renderer, Vector2(i, currentRow), m_pPlayer->GetPlayerStorage(), "assets//sprites//level//SafeArea.png", WorldItems.size());
					m_safeZoneCenter = { static_cast<float>(i), static_cast<float>(currentRow) };
				}
			}

			if (line[i] == '\t')
			{
				LogManager::GetInstance().Log("THERE IS A TAB INSIDE OF THE TILE TEXT FILE, REPLACE IT WITH SPACES OR THE LEVEL WILL FAIL TO LOAD CORRECTLY!");
			}
		}

		//Increases the row
		currentRow--;
	}

	//Resets the current row variables to reuse for the next while loop
	currentRow = 0;

	//Closes the file reader
	levelTileDataPos.close();

	currentRow = 0;
}

void
SceneBox2DLevel::LoadItems(Renderer& renderer)
{
	std::ifstream levelItemsDataPos;
	std::string line;

	levelItemsDataPos.open("assets\\levels\\items.loc");

	//Creates a variable for the current row being iterated through
	int currentRow = 0;

	//Go through the text files contents saving each character to use as a tile determiner for the map tiles
	b2BodyDef bodyDef2;
	b2PolygonShape groundShape;
	b2FixtureDef fixtureDef2;

	//Resets the current row variables to reuse for the next while loop
	currentRow = 0;

	//Go through the text files contents saving each character to use as a tile determiner for the item tiles
	while (std::getline(levelItemsDataPos, line))
	{
		for (std::string::size_type i = 0; i < line.size(); i++)
		{
			if (line[i] == itemOne)
			{
				m_pCollectable = new Collectable;
				m_pCollectable->Initialise(renderer, *m_pPlayer, Vector2(i, currentRow));

				WorldItems.push_back(m_pCollectable);
			}
		}

		//Increases the row
		currentRow--;
	}

	//Closes the file reader
	levelItemsDataPos.close();

	currentRow = 0;
}

void
SceneBox2DLevel::LoadEnemies(Renderer& renderer)
{
	std::ifstream levelEnemyDataPos;
	INI::Settings levelEnemyData;
	std::string line;

	levelEnemyDataPos.open("assets\\levels\\enemy.loc");
	levelEnemyData.LoadFile("assets\\levels\\enemy.dta");

	//Creates a variable for the current row being iterated through
	int currentRow = 0;

	//Go through the text files contents saving each character to use as a tile determiner for the map tiles
	b2BodyDef bodyDef2;
	b2PolygonShape groundShape;
	b2FixtureDef fixtureDef2;

	line = "";

	int enemyIndex = 0;

	//Go through the text files contents saving each character to use as a tile determiner for the item tiles
	while (std::getline(levelEnemyDataPos, line))
	{
		for (std::string::size_type i = 0; i < line.size(); i++)
		{
			if (line[i] == enemy)
			{
				char enemyCNodeString[16];
				sprintf_s(enemyCNodeString, "%dcnodes", enemyIndex);
				int cnodes = levelEnemyData.GetInt("enemy", enemyCNodeString);
				PathNode* nodes = new PathNode[cnodes];
				for (int nodeIndex = 0; nodeIndex < cnodes; ++nodeIndex)
				{
					char nodePosStringIdentifier[8];
					char nodeConStringIdentifier[8];
					sprintf_s(nodePosStringIdentifier, "%dx%dpos", enemyIndex, nodeIndex);
					sprintf_s(nodeConStringIdentifier, "%dx%dcon", enemyIndex, nodeIndex);

					nodes[nodeIndex].nextNode = &nodes[levelEnemyData.GetInt("enemy", nodeConStringIdentifier)];

					bool isX = true;
					std::stringstream stream(levelEnemyData.GetString("enemy", nodePosStringIdentifier));
					std::string coord;
					while (std::getline(stream, coord, ',')) {
						if (isX)
						{
							nodes[nodeIndex].position.x = atoi(coord.c_str());
							isX = false;
						}
						else
						{
							nodes[nodeIndex].position.y = -atoi(coord.c_str());
						}

					}

				}

				Enemy* enemy = new Enemy();
				enemy->Initialise(renderer);
				enemy->Setup(*m_pPlayer, nodes, *m_pWorld, m_pathFinder);
				enemy->SetPosition(Vector2(i, currentRow));
				WorldEnemies.push_back(enemy);
				++enemyIndex;
			}
			else if (line[i] == player)
			{
				//TODO: SPAWN THE PLAYER
				m_pPlayer->SetPosition(Vector2(i, currentRow));
			}
		}

		//Increases the row
		currentRow--;
	}

	//Closes the file reader
	levelEnemyDataPos.close();
	levelEnemyData.Unload();
}

void
SceneBox2DLevel::LoadNavMesh()
{
	std::ifstream levelTileDataPos;
	std::string line;
	bool extendingCurrentWall = false;
	NavMesh::Polygon navMeshPolygon;

	levelTileDataPos.open("assets\\levels\\navmesh.loc");

	int currentRow = 0;

	std::vector<NavMesh::Polygon> polygons;
	while (std::getline(levelTileDataPos, line))
	{

		for (std::string::size_type i = 0; i < line.size(); i++)
		{
			if (line[i] == 'x' || line[i] == 'y')
			{
				if (!extendingCurrentWall) 
				{
					navMeshPolygon.AddPoint(i - 1.75f, currentRow + 1.75f);
					navMeshPolygon.AddPoint(i - 1.75f, currentRow - 1.75f);
					extendingCurrentWall = true;
				}
			}
			else // if current char != x || y
			{
				if (extendingCurrentWall)
				{
					navMeshPolygon.AddPoint(i + 0.75f, currentRow + 1.75f);
					navMeshPolygon.AddPoint(i + 0.75f, currentRow - 1.75f);
					polygons.push_back(navMeshPolygon);
					navMeshPolygon.Clear();
					extendingCurrentWall = false;
				}
			}
		}

		// If reached end of line and current wall is not complete
		if (extendingCurrentWall)
		{
			navMeshPolygon.AddPoint(line.size() + 0.75f, currentRow + 1.75f);
			navMeshPolygon.AddPoint(line.size(), currentRow - 1.75f);
			polygons.push_back(navMeshPolygon);
			navMeshPolygon.Clear();
		}
		currentRow--;
	}


	levelTileDataPos.close();
	m_pathFinder.AddPolygons(polygons, m_pPlayer->GetCollisionRadius());

	currentRow = 0;
}

void
SceneBox2DLevel::LoadWallCollision(Renderer& renderer)
{
	std::ifstream levelCollisionDataPos;
	std::string line;

	levelCollisionDataPos.open("assets\\levels\\wallPhysics.loc");

	//Creates a variable for the current row being iterated through
	int currentRow = 0;
	m_pCollectable = new Collectable;

	//Go through the text files contents saving each character to use as a tile determiner for the map tiles
	b2BodyDef bodyDef2;
	b2PolygonShape groundShape;
	b2FixtureDef fixtureDef2;

	//Resets the current row variables to reuse for the next while loop
	currentRow = 0;

	//Go through the text files contents saving each character to use as a tile determiner for the item tiles
	while (std::getline(levelCollisionDataPos, line))
	{
		for (std::string::size_type i = 0; i < line.size(); i++)
		{
			tempVector.push_back(line[i]);

			if (line[i] == itemOne)
			{
				m_pCollectable->Initialise(renderer, *m_pPlayer, Vector2(i, currentRow));

				WorldItems.push_back(m_pCollectable);
			}
			else if (line[i] == loadLevel1)
			{
				levelOneLoad = new CurrentLevelLoaded;
				levelOneLoad->Initialise(renderer, Vector2(i, currentRow));
			}
			else if (line[i] == loadLevel2)
			{
				levelTwoLoad = new CurrentLevelLoaded;
				levelTwoLoad->Initialise(renderer, Vector2(i, currentRow));
			}
			else if (line[i] == loadLevel3)
			{
				levelThreeLoad = new CurrentLevelLoaded;
				levelThreeLoad->Initialise(renderer, Vector2(i, currentRow));
			}
		}

		textFileDataStorage.push_back(tempVector);
		tempVector.clear();

		//Increases the row
		currentRow--;
	}

	int collisionCount;
	bool directionLeft = true;
	collisionCreation = new CollisionCreation;

	for (unsigned int i = 0; i < textFileDataStorage.size(); i++)
	{
		collisionCount = 0;

		for (unsigned int k = 0; k < textFileDataStorage[i].size(); k++)
		{
			if (textFileDataStorage[i][k] == 'x')
			{
				collisionCount++;
			}
			
			else if (textFileDataStorage[i][k] != 'x' && collisionCount > 0)
			{
				//Create the box2dObject...
				collisionCreation->Initialise(Vector2(k, i), collisionCount, directionLeft, renderer);

				collisionCount = 0;
			}
		}
	}

	collisionCount = 0;
	directionLeft = false;

	for (unsigned int k = 0; k < textFileDataStorage[0].size(); k++)
	{
		for (unsigned int i = 0; i < textFileDataStorage.size(); i++)
		{
			if (textFileDataStorage[i][k] == 'y')
			{
				collisionCount++;
			}

			if (textFileDataStorage[i][k] != 'y' && collisionCount > 0)
			{
				//Create the box2dObject...
				collisionCreation->Initialise(Vector2(k, i), collisionCount, directionLeft, renderer);

				collisionCount = 0;
			}
		}
	}



	//Closes the file reader
	levelCollisionDataPos.close();

	currentRow = 0;
}
