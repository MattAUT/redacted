#include "currentLevelLoaded.h"

#include <engine/renderer.h>
#include <engine/sprite.h>
#include <engine/spritesheetanimation.h>
#include <engine/input.h>
#include <engine/xinputcontroller.h>
#include <engine/camera.h>
#include <engine/entity.h>
#include <engine/soundmanager.h>
#include <engine/scenemanager.h>
#include <logmanager.h>
#include <box2d/box2d.h>

#ifdef _DEBUG
#include <imgui/imgui_impl_sdl.h>
#endif // _DEBUG

#include <utils.h>
#include <limits>
#include <fmod.hpp>
#include "vector2.h"

#include "playerstorage.h"
#include "collectable.h"
#include "abilitymanager.h"

CurrentLevelLoaded::CurrentLevelLoaded()
{

}

CurrentLevelLoaded::~CurrentLevelLoaded()
{
	for (Collectable* item : m_vStash)
	{
		delete item;
	}
	m_vStash.clear();
}

bool
CurrentLevelLoaded::Initialise(Renderer& renderer, Vector2 position)
{
	m_position = position;

	//Fixture def Sensor
	b2PolygonShape boxSensor;
	boxSensor.SetAsBox(0.5f, 0.5f);

	m_pUserData = new UserData;
	m_pUserData->m_pEntityPointer = this;
	m_pUserData->m_type = ET_SAFEAREA;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxSensor;
	fixtureDef.isSensor = true;
	fixtureDef.userData.pointer = (uintptr_t)m_pUserData;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(m_position.x, m_position.y);
	bodyDef.userData.pointer = (uintptr_t)m_pUserData;
	bodyDef.gravityScale = 0;
	m_pB2Body = SceneManager::GetInstance().m_pWorld->CreateBody(&bodyDef);

	m_pSafeAreaDetector = m_pB2Body->CreateFixture(&fixtureDef);

	return true;
}

void
CurrentLevelLoaded::Process(float deltaTime, Input& input)
{

}

void CurrentLevelLoaded::Draw(Renderer & renderer)
{

}

#ifdef _DEBUG
void
CurrentLevelLoaded::DebugDraw()
{

}
#endif _DEBUG

void
CurrentLevelLoaded::OnCollision(UserData* type)
{
	if (type->m_type == ET_PLAYER)
	{
		isPlayerInArea = true;
	}
}

void
CurrentLevelLoaded::OnCollisionExit(UserData* type)
{
	if (type->m_type == ET_PLAYER)
	{
		isPlayerInArea = false;
	}
}

bool
CurrentLevelLoaded::GetPlayerInArea()
{
	return isPlayerInArea;
}