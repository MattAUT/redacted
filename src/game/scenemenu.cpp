// COMP710 GP Framework 2022

#include "scenemenu.h"

#include <engine/renderer.h>
#include <engine/sprite.h>
#include <engine/scenemanager.h>
#include <engine/soundmanager.h>
#include <engine/xinputcontroller.h>

#include "mainmenu.h"

#include <cassert>
#include <utils.h>
#include <cmath>
#include <fmod.hpp>
#ifdef _DEBUG
#include <imgui/imgui_impl_sdl.h>
#endif // _DEBUG

SceneMenu::SceneMenu()
{
#ifdef _DEBUG
	dm_name = "Menu Scene";
#endif // _DEBUG
}

SceneMenu::~SceneMenu()
{

}

bool SceneMenu::Initialise(Renderer& renderer, Camera& camera)
{
	m_pMainMenu = new MainMenu;
	m_pMainMenu->Initialise(renderer);

	return Scene::Initialise(renderer, camera);
}

void SceneMenu::Process(float deltaTime, Input& input)
{
	m_pMainMenu->Process(deltaTime, input);
}

void SceneMenu::Draw(Renderer& renderer)
{
	m_pMainMenu->Draw(renderer);
}

void
SceneMenu::Reset()
{

}

#ifdef _DEBUG
void SceneMenu::DebugDraw(Renderer& renderer)
{

}
#endif // _DEBUG

void SceneMenu::SetCurrentLevel(int level) {

}