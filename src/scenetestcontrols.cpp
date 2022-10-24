#include "scenetestcontrols.h"

#include <engine/renderer.h>
#include <engine/sprite.h>
#include <engine/scenemanager.h>
#include <engine/xinputcontroller.h>
#include <imgui/imgui_impl_sdl.h>

SceneControlerTest::SceneControlerTest()
{
#ifdef _DEBUG
	dm_name = "Controls Test Scene";
#endif // _DEBUG
}

SceneControlerTest::~SceneControlerTest()
{
	if (m_bIsLoaded)
	{
		Deinitialise();
	}
}

bool SceneControlerTest::Initialise(Renderer & renderer, Camera & camera)
{


	return Scene::Initialise(renderer, camera);
}

void SceneControlerTest::Deinitialise()
{
	Scene::Deinitialise();
}

void SceneControlerTest::Process(float deltaTime, Input & input)
{
}

void SceneControlerTest::Draw(Renderer & renderer)
{
}

void SceneControlerTest::Reset()
{
}

void SceneControlerTest::DebugDraw(Renderer & renderer)
{
}

void SceneControlerTest::BeginDebugDraw()
{
}

void SceneControlerTest::AddEntityToDebugQueue(Entity * entity)
{
}

void SceneControlerTest::RemoveEntityFromDebugQueue(Entity * rmEntity)
{
}
