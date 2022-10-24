#ifndef __SCENELEVELGENERATORTEST_H__
#define __SCENELEVELGENERATORTEST_H__

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

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

class SceneLevelGeneratorTest : public Scene
{

	// Member Methods
public:
	SceneLevelGeneratorTest();
	virtual ~SceneLevelGeneratorTest();

	virtual bool Initialise(Renderer& renderer, Camera& camera) override;
	virtual void Process(float deltaTime, Input& input) override;
	virtual void Draw(Renderer& renderer) override;
	virtual void Reset() override;
	virtual void SetCurrentLevel(int level);
#ifdef _DEBUG
	virtual void DebugDraw(Renderer& renderer) override;
#endif // _DEBUG

protected:

private:
	SceneLevelGeneratorTest(const SceneLevelGeneratorTest& s);
	SceneLevelGeneratorTest& operator=(const SceneLevelGeneratorTest& s) {}

	// Member Data
public:
protected:
	std::vector<Sprite*> WorldTiles;
	std::vector<Collectable*> WorldItems;
	std::vector<Enemy*> WorldEnemies;

	std::vector<Vector2*> WorldTilesLocations;
	std::vector<Vector2*> WorldItemsLocations;
	std::vector<Vector2*> WorldEnemyLocations;
	NavMesh::PathFinder m_pathFinder;

	Sprite* mapTiles;
	Vector2* tileLocations;
	SafeArea* safeArea;

	Player* m_pPlayer;
	b2World* m_pWorld;
	b2Body* m_testGroundBody;
	b2Fixture* m_pTestFixture;

	Collectable* m_pCollectable;

	int currentLevel;
private:

};

#endif // __SCENELEVELGENERATORTEST_H__

