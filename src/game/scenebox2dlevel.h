#ifndef __SCENEBOX2DLEVEL_H__
#define __SCENEBOX2DLEVEL_H__

#include <engine/scene.h>
#include <vector2.h>
#include <path_finder.h>

// Forward Declarations
class SceneManager;
class Sprite;
class b2World;
class Player;
class Enemy;
class Collectable;
class Enemy;
class SafeArea;
class TileContainer;
class ParticleEmitter;
class HUD;
class CollisionCreation;
class CurrentLevelLoaded;
class PauseScreen;

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

class SceneBox2DLevel : public Scene
{

	// Member Methods
public:
	SceneBox2DLevel();
	virtual ~SceneBox2DLevel();

	virtual bool Initialise(Renderer& renderer, Camera& camera) override;
	virtual void Process(float deltaTime, Input& input) override;
	virtual void Draw(Renderer& renderer) override;
	virtual void Reset() override;
	virtual void SetCurrentLevel(int level);

	void LoadLevel(Renderer& renderer);
	void LoadItems(Renderer& renderer);
	void LoadEnemies(Renderer& renderer);
	void LoadWallCollision(Renderer& renderer); \
	void LoadNavMesh();

#ifdef _DEBUG
	virtual void DebugDraw(Renderer& renderer) override;
#endif // _DEBUG

protected:

private:
	SceneBox2DLevel(const SceneBox2DLevel& s);
	SceneBox2DLevel& operator=(const SceneBox2DLevel& s) {}

	// Member Data
public:
protected:
	std::vector<Sprite*> WorldTiles;
	std::vector<Collectable*> WorldItems;
	std::vector<Enemy*> WorldEnemies;

	//std::vector<Tile*> newTiles;
	//TileContainer* safeZone;
	TileContainer* tileContainer;
	TileContainer* tileContainer1;
	TileContainer* tileContainer2;
	TileContainer* safeAreaTileContainer;
	Vector2 m_safeZoneCenter;
	NavMesh::PathFinder m_pathFinder;
	Sprite* mapTiles;
	SafeArea* safeArea;
	Player* m_pPlayer;
	b2World* m_pWorld;
	b2Body* m_testGroundBody;
	b2Fixture* m_pTestFixture;
	CollisionCreation* collisionCreation;
	Collectable* m_pCollectable;
	ParticleEmitter* m_pParticleEmitter;
	HUD* gameHUD;
	PauseScreen* m_pPauseScreen;

	int currentLevel;

	std::vector<std::vector<char>> textFileDataStorage;
	std::vector<char> tempVector;
	bool isLevelComplete;

	CurrentLevelLoaded* levelOneLoad;
	CurrentLevelLoaded* levelTwoLoad;
	CurrentLevelLoaded* levelThreeLoad;

private:

};

#endif // __SCENEBOX2DLEVEL_H__

