#include "safearea.h"

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

SafeArea::SafeArea()
	: isPlayerInSafeArea(false)
{

}

SafeArea::~SafeArea()
{
	for (Collectable* item : m_vStash)
	{
		delete item;
	}
	m_vStash.clear();

	delete m_pSprite;
	m_pSprite = 0;

	//delete m_pDebugSprite;
	//m_pDebugSprite = 0;

	delete m_pUserData;
	m_pUserData = 0;
}

bool
SafeArea::Initialise(Renderer& renderer, Vector2 position, PlayerStorage* playerStorage, const char* safeAreaFilename, int numOfItemComplete)
{
	m_iNumOfItemComplete = numOfItemComplete;

	m_pSprite = renderer.CreateSprite(safeAreaFilename, SpriteOrigin::CENTER);

	m_pPlayerStorage = playerStorage;

	m_position = position;

	// Box2D Init
	m_pUserData = new UserData;
	m_pUserData->m_pEntityPointer = this;
	m_pUserData->m_type = ET_SAFEAREA;

	//Fixture def Sensor
	b2PolygonShape boxSensor;
	boxSensor.SetAsBox(0.5f, 0.5f);

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
SafeArea::Process(float deltaTime, Input& input)
{

}

void
SafeArea::Draw(Renderer& renderer)
{
	m_pSprite->Draw(renderer, m_position, 0.f);
}

#ifdef _DEBUG
void
SafeArea::DebugDraw()
{
}
#endif _DEBUG

void
SafeArea::StoreCollectables(PlayerStorage* storage)
{
	std::vector<Collectable*> pouch = storage->GetPouch();
	m_vStash.insert(m_vStash.end(), pouch.begin(), pouch.end());
	storage->ClearPouch();
}

void
SafeArea::OnCollision(UserData* userData)
{
	if (userData->m_type == ET_PLAYER)
	{
		StoreCollectables(m_pPlayerStorage);
		isPlayerInSafeArea = true;
	}

}

void
SafeArea::OnCollisionExit(UserData* userData)
{
	if (userData->m_type == ET_PLAYER)
	{
		isPlayerInSafeArea = false;
	}
}

int
SafeArea::GetStashCount()
{
	return m_vStash.size();
}


int
SafeArea::GetObjectiveCount()
{
	return m_iNumOfItemComplete;
}

bool
SafeArea::isLevelComplete()
{
	return m_vStash.size() >= m_iNumOfItemComplete;
}