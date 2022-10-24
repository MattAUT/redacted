#include "scenemanager.h"

#include <game/game.h>
#include "renderer.h"
#include "scene.h"
#include "logmanager.h"
#include <imgui/imgui_impl_sdl.h>

#include "collisionlistener.h"
SceneManager* SceneManager::sm_pInstance = nullptr;

SceneManager &SceneManager::GetInstance() {
	if (sm_pInstance == nullptr) {
		sm_pInstance = new SceneManager();
	}

	return (*sm_pInstance);
}

void SceneManager::DestroyInstance() {
	delete sm_pInstance;
	sm_pInstance = nullptr;
}

SceneManager::SceneManager()
	: m_iActiveScene(0)
	, m_iSceneToChangeToo(0)
{

}

SceneManager::~SceneManager()
{
	for (unsigned int i = 0; i < sm_uiSceneCount; ++i)
	{
		delete m_pScenes[i];
		m_pScenes[i] = nullptr;
	}

	delete m_pWorld;
	m_pWorld = 0;
}

bool
SceneManager::Initialise(Game& game, Scene** scenes, Camera& camera, Renderer& renderer)
{
	m_pGame = &game;
	m_pCamera = &camera;
	m_pRenderer = &renderer;

	//Init b2world
	b2Vec2 gravity(0.0f, 0.0f);
	m_pWorld = new b2World(gravity);
	m_pWorld->SetContactListener(new CollisionListener); // TODO: Store pointer for Listener so it can be deleted
	for (unsigned int i = 0; i < sm_uiSceneCount; ++i)
	{
		m_pScenes[i] = scenes[i];
		m_pScenes[i]->Initialise(*m_pRenderer, *m_pCamera);
	}

	LogManager::GetInstance().Log("SceneManager Initalised");
	return true;
}

#ifdef _DEBUG
void SceneManager::DebugDraw()
{
	// Scene select
	ImGui::Begin("Scene Select");
	ImGui::SliderInt("Scene", &m_iSceneToChangeToo, 0, SceneManager::sm_uiSceneCount -1);
	if (ImGui::Button("Change Scene")) 
	{
		SwitchScenes(m_iSceneToChangeToo);
	}
	ImGui::End();
}
#endif _DEBUG

bool
SceneManager::SwitchScenes(unsigned int index)
{
	SceneManager& manager = SceneManager::GetInstance();
	if (index > sm_uiSceneCount - 1)
	{
		return false;
	}

	manager.m_iActiveScene = index;
	return true;
}

Scene& 
SceneManager::GetActiveScene()
{
	SceneManager& manager = SceneManager::GetInstance();
	return *(manager.m_pScenes[manager.m_iActiveScene]);
}

void
SceneManager::Exit()
{
	m_pGame->Quit();
}

Camera*
SceneManager::GetCamera()
{
	return m_pCamera;
}
