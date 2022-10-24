#include "scenecontrollertest.h"

#include <engine/renderer.h>
#include <engine/sprite.h>
#include <engine/scenemanager.h>
#include <engine/xinputcontroller.h>
#include <box2d/box2d.h>
#include <logmanager.h>

#include "player.h"
#include "enemy.h"
#include "collectable.h"
#include "test.h"

#ifdef _DEBUG
#include <imgui/imgui_impl_sdl.h>
#endif // _DEBUG

SceneControlerTest::SceneControlerTest()
{
#ifdef _DEBUG
	dm_name = "Controls Test Scene";
#endif // _DEBUG
}

SceneControlerTest::~SceneControlerTest()
{
	delete m_pPlayer;
	m_pPlayer = 0;
}

bool SceneControlerTest::Initialise(Renderer & renderer, Camera & camera)
{
	m_pPlayer = new Player;
	m_pPlayer->Initialise(renderer);

	m_pEnemy = new Enemy();
	m_pEnemy->Initialise(renderer);

	return Scene::Initialise(renderer, camera);
}

void SceneControlerTest::Process(float deltaTime, Input & input)
{
	m_pPlayer->Process(deltaTime, input);
	m_pCollectable->Process(deltaTime, input);
}

void SceneControlerTest::Draw(Renderer & renderer)
{
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	m_pPlayer->Draw(renderer);
	m_pEnemy->Draw(renderer);
	m_pCollectable->Draw(renderer);
}

void SceneControlerTest::Reset()
{
}

#ifdef _DEBUG
void SceneControlerTest::DebugDraw(Renderer & renderer)
{
	m_pPlayer->DebugDraw();
	m_pCollectable->DebugDraw();
}
#endif _DEBUG