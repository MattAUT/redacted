// COMP710 GP Framework 2022
#ifndef __SCENECONTROLLERTEST_H__
#define __SCENECONTROLLERTEST_H__

#include <engine/scene.h>
#include <vector>

// Forward Declarations:
class Renderer;
class Camera;
class Input;
class SceneManager;
class Entity;
class Sprite;
class Player;
class b2BodyDef;
class Collectable;
class Test;
class Enemy;

namespace FMOD
{
	class System;
}

class SceneControlerTest : public Scene
{
	// Member Methods:
public:
	SceneControlerTest();
	virtual ~SceneControlerTest();

	virtual bool Initialise(Renderer& renderer, Camera& camera) override;
	virtual void Process(float deltaTime, Input& input) override;
	virtual void Draw(Renderer& renderer) override;
	virtual void Reset() override;

#ifdef _DEBUG
	virtual void DebugDraw(Renderer& renderer) override;
	//// Any ImGui elements can be appened to the end of this method
	//// call, as long as ImGui::End is called sometime after. 
	//virtual void BeginDebugDraw();
	//void AddEntityToDebugQueue(Entity* entity);
	//void RemoveEntityFromDebugQueue(Entity* rmEntity);
#endif // _DEBUG

protected:
private:
	SceneControlerTest(const SceneControlerTest& scene);
	SceneControlerTest& operator=(const SceneControlerTest& scene) {}

	// Member Data:
public:
protected:

	Player* m_pPlayer;
	Enemy* m_pEnemy;
	Collectable* m_pCollectable;
	Test* m_pTest;
private:

};

#endif // __SCENECONTROLLERTEST_H__
