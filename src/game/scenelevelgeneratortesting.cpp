// COMP710 GP Framework 2022

#include "scenelevelgeneratortesting.h"

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

#ifdef _DEBUG
#include <imgui/imgui_impl_sdl.h>
#endif // _DEBUG

SceneLevelGeneratorTest::SceneLevelGeneratorTest() :
	currentLevel(0)
{
#ifdef _DEBUG
	dm_name = "Menu Scene";
#endif // _DEBUG
}

SceneLevelGeneratorTest::~SceneLevelGeneratorTest()
{
	delete m_pPlayer;
	m_pPlayer = 0;

	delete tileLocations;
	tileLocations = 0;

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

	for (Vector2* v2 : WorldTilesLocations)
	{
		delete v2;
	}
	WorldTilesLocations.clear();

	for (Vector2* v2 : WorldItemsLocations)
	{
		delete v2;
	}
	WorldItemsLocations.clear();

}

bool SceneLevelGeneratorTest::Initialise(Renderer& renderer, Camera& camera)
{
	//TODO: Maybe add a save??
	//Sets the current level to 1 by default if no level is loaded
	if (currentLevel == 0)
	{
		currentLevel = 1;
	}

	m_pWorld = SceneManager::GetInstance().m_pWorld;

	m_pPlayer = new Player;
	m_pPlayer->Initialise(renderer);

	safeArea = new SafeArea;
	safeArea->Initialise(renderer, Vector2(3, -3), m_pPlayer->GetPlayerStorage(), "assets/sprites/level/SafeArea.png", 0);



	std::ifstream levelTileDataPos;
	std::ifstream levelItemsDataPos;
	std::ifstream levelEnemyDataPos;
	INI::Settings levelEnemyData;
	std::string line;

	//Will set the files to be opened
	if (currentLevel == 1)
	{
		levelTileDataPos.open("assets\\levels\\tiles.loc");
		levelItemsDataPos.open("assets\\levels\\items.loc");
		levelEnemyDataPos.open("assets\\levels\\enemy.loc");
		levelEnemyData.LoadFile("assets\\levels\\enemy.dta");
	}
	else if (currentLevel == 2)
	{
		//myFile.open("tilesLayout/tiles.loc");
		//myFileItems.open("tilesLayout/items.loc");
		//myFileEnemies.open("tilesLayout/enemies.loc");
	}
	else if (currentLevel == 3)
	{
		//myFile.open("tilesLayout/tiles.loc");
		//myFileItems.open("tilesLayout/items.loc");
		//myFileEnemies.open("tilesLayout/enemies.loc");
	}
	else
	{
		//myFile.open("tilesLayout/tiles.txt");
		//myFileItems.open("tilesLayout/items.loc");
		//myFileEnemies.open("tilesLayout/enemies.loc");
	}

	//Creates a variable for the current row being iterated through
	int currentRow = 0;

	//Go through the text files contents saving each character to use as a tile determiner for the map tiles
	std::vector<NavMesh::Polygon> polygons;
	while (std::getline(levelTileDataPos, line))
	{
		for (std::string::size_type i = 0; i < line.size(); i++)
		{
			//Initializes a new sprite
			mapTiles = new Sprite;

			// Initializes a new vector2 for the sprites location and saves it into a vector
			tileLocations = new Vector2;

			//Checks what type of tile should be generated for the above sprite
			if (line[i] == '1')
			{
				mapTiles = renderer.CreateSprite("assets\\sprites\\level\\border.png", SpriteOrigin::CENTER);
				WorldTiles.push_back(mapTiles);

				//TODO: ADD A COMM3NT D1CK H34D 
				b2BodyDef bodyDef2;
				bodyDef2.position.Set((i), (currentRow));
				bodyDef2.userData.pointer = (uintptr_t)this;
				m_testGroundBody = m_pWorld->CreateBody(&bodyDef2);

				//Fixture def
				b2PolygonShape groundShape;
				groundShape.SetAsBox(0.5, 0.5);

				b2FixtureDef fixtureDef2;
				fixtureDef2.shape = &groundShape;
				fixtureDef2.friction = 0.3f;
				fixtureDef2.userData.pointer = (uintptr_t)this;

				m_pTestFixture = m_testGroundBody->CreateFixture(&fixtureDef2);

				// Create NavMesh Geometry
				NavMesh::Polygon navMeshPolygon;
				navMeshPolygon.AddPoint(i - 0.5f, currentRow + 0.5f);
				navMeshPolygon.AddPoint(i + 0.5f, currentRow + 0.5f);
				navMeshPolygon.AddPoint(i + 0.5f, currentRow - 0.5f);
				navMeshPolygon.AddPoint(i - 0.5f, currentRow - 0.5f);
				polygons.push_back(navMeshPolygon);
			}
			else if (line[i] == '0')
			{
				mapTiles = renderer.CreateSprite("assets\\sprites\\level\\open.png", SpriteOrigin::CENTER);
				WorldTiles.push_back(mapTiles);
			}
			else if (line[i] == '2')
			{
				m_pPlayer->SetPosition(Vector2((i), (currentRow)));
				mapTiles = renderer.CreateSprite("assets\\sprites\\level\\open.png", SpriteOrigin::CENTER);
				WorldTiles.push_back(mapTiles);
			}

			if (line[i] != ' ')
			{
				tileLocations->Set((i), (currentRow));
				WorldTilesLocations.push_back(tileLocations);
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

	// Set up NavMesh geometry
	m_pathFinder.AddPolygons(polygons, m_pPlayer->GetCollisionRadius());

	line = "";

	//Go through the text files contents saving each character to use as a tile determiner for the item tiles
	while (std::getline(levelItemsDataPos, line))
	{
		for (std::string::size_type i = 0; i < line.size(); i++)
		{
			// Initializes a new vector2 for the sprites location and saves it into a vector
			tileLocations = new Vector2;

			if (line[i] == '3')
			{
				m_pCollectable = new Collectable;
				m_pCollectable->Initialise(renderer, *m_pPlayer, Vector2(i, currentRow));

				WorldItems.push_back(m_pCollectable);
			}

			tileLocations->Set((i), (currentRow));
			WorldItemsLocations.push_back(tileLocations);
		}

		//Increases the row
		currentRow--;
	}

	//Closes the file reader
	levelItemsDataPos.close();

	int enemyIndex = 0;
	line = "";
	currentRow = 0;

	//Go through the text files contents saving each character to use as a tile determiner for the item tiles
	while (std::getline(levelEnemyDataPos, line))
	{
		for (std::string::size_type i = 0; i < line.size(); i++)
		{
			if (line[i] == '4')
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

			tileLocations->Set((i), (currentRow));
			WorldEnemyLocations.push_back(tileLocations);
		}

		//Increases the row
		currentRow--;
	}

	//Closes the file reader
	levelEnemyDataPos.close();
	levelEnemyData.Unload();

	return Scene::Initialise(renderer, camera);
}

void SceneLevelGeneratorTest::Process(float deltaTime, Input& input)
{
	//m_pCamera->SetPosition(m_pPlayer->GetPosition());
	m_pPlayer->Process(deltaTime, input);
	m_pCamera->SetPosition(m_pPlayer->GetPosition());
	//m_pCollectable->Process(deltaTime, input);

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
			SceneManager::GetInstance().m_pWorld->DestroyBody(WorldItems[num]->GetBody());
			WorldItems[num]->SetIsToBeDeleted(false);
		}
	}

	for (unsigned int num = 0; num < WorldEnemies.size(); num++)
	{
		//WorldEnemies[num]->Process(deltaTime, input);
	}
}

void SceneLevelGeneratorTest::Draw(Renderer& renderer)
{
	//Draws the tiles
	for (unsigned int num = 0; num < WorldTiles.size(); num++)
	{
		WorldTiles[num]->Draw(renderer, *WorldTilesLocations[num], 0.0f);
	}

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

	m_pCollectable->Draw(renderer);
}

void
SceneLevelGeneratorTest::Reset()
{
	WorldTiles.clear();
	WorldItems.clear();
	WorldEnemies.clear();

	//Will delete the vector items and delete the memory associated with the scene
	WorldTilesLocations.clear();
	WorldItemsLocations.clear();
	WorldEnemyLocations.clear();
}

#ifdef _DEBUG
void SceneLevelGeneratorTest::DebugDraw(Renderer& renderer)
{
	m_pPlayer->DebugDraw();
	m_pCollectable->DebugDraw();

	safeArea->DebugDraw();

	ImGui::Begin("Enemies");
	for (unsigned int num = 0; num < WorldEnemies.size(); num++)
	{
		WorldEnemies[num]->DebugDraw();
	}
	ImGui::End();
}
#endif // _DEBUG

void SceneLevelGeneratorTest::SetCurrentLevel(int level) {
	currentLevel = level;
}