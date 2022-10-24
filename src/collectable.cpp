#include "collectable.h"

#include <engine/renderer.h>
#include <engine/sprite.h>
#include <engine/spritesheetanimation.h>
#include <engine/input.h>
#include <engine/xinputcontroller.h>
#include <engine/entity.h>
#include <engine/scenemanager.h>
#include <engine/soundmanager.h>
#include <logmanager.h>
#include <box2d/box2d.h>

#ifdef _DEBUG
#include <imgui/imgui_impl_sdl.h>
#endif // _DEBUG

#include <utils.h>
#include <limits>
#include <fmod.hpp>
#include "vector2.h"
#include "player.h"

Collectable::Collectable()
	: m_bIsCollected(false)
	, m_bIsPlayerInRange(false)
	, m_bIsPlayerFull(false)
	, m_bIsHeld(false)
	, m_pPickUpSensor(0)
	, m_fSpeed(10)
{
}



Collectable::~Collectable()
{
	delete m_pSprite;
	m_pSprite = 0;

	delete m_pUserData;
	m_pUserData = 0;

	delete sensorUserData;
	sensorUserData = 0;

	delete m_pCollectionArea;
	m_pCollectionArea = 0;
}

bool
Collectable::Initialise(Renderer& renderer, Player& player, Vector2 position)
{
	if (!Entity::Initialise(renderer, SoundManager::GetInstance()))
	{
		return false;
	}

	m_position = position;


	m_pSprite = renderer.CreateSprite("assets\\sprites\\entities\\bullet.png", SpriteOrigin::CENTER);
	m_pSprite->SetRedTint(1);
	m_pSprite->SetGreenTint(1);
	m_pSprite->SetBlueTint(0);

	m_pCollectionArea = renderer.CreateSprite("assets\\sprites\\entities\\ball.png", SpriteOrigin::CENTER);
	m_pCollectionArea->SetRedTint(1);
	m_pCollectionArea->SetGreenTint(1);
	m_pCollectionArea->SetBlueTint(0);
	m_pCollectionArea->SetAlpha(0.5);
	m_pCollectionArea->SetScale(2);
	// B2Body init
	m_pUserData = new UserData;
	m_pUserData->m_pEntityPointer = this;
	m_pUserData->m_type = ET_COLLECTABLE;

	sensorUserData = new UserData;
	sensorUserData->m_pEntityPointer = this;
	sensorUserData->m_type = ET_COLLECTSENSOR;

	//Body def
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(m_position.x, m_position.y);
	bodyDef.userData.pointer = (uintptr_t)m_pUserData;
	bodyDef.gravityScale = 0;
	m_pB2Body = SceneManager::GetInstance().m_pWorld->CreateBody(&bodyDef);


	//Fixture def Collectable
	b2CircleShape dynamicCircle;
	dynamicCircle.m_radius = Renderer::ScreenSpaceToWorldSpace(m_pSprite->GetHeight() / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicCircle;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.userData.pointer = (uintptr_t)m_pUserData;

	m_pB2Fixture = m_pB2Body->CreateFixture(&fixtureDef);

	//Fixture def Sensor
	b2CircleShape dynamicCircle2;
	dynamicCircle2.m_radius = Renderer::ScreenSpaceToWorldSpace(m_pCollectionArea->GetHeight() / 2);

	b2FixtureDef fixtureDef2;
	fixtureDef2.shape = &dynamicCircle2;
	fixtureDef2.isSensor = true;
	fixtureDef2.userData.pointer = (uintptr_t)sensorUserData;

	m_pPickUpSensor = m_pB2Body->CreateFixture(&fixtureDef2);

	m_pPlayer = &player;

	// Sound
	SoundManager::GetInstance().createSound("assets\\sounds\\items\\item.mp3", FMOD_DEFAULT, 0, &m_pCoinSound);

	return true;
}

void
Collectable::Process(float deltaTime, Input& input)
{
	m_position.x = m_pB2Body->GetWorldCenter().x;
	m_position.y = m_pB2Body->GetWorldCenter().y;

	if (m_bIsPlayerInRange && !m_bIsPlayerFull && !m_bIsCollected)
	{
		float x = m_pPlayer->GetPosition().x - m_pB2Body->GetPosition().x;
		float y = m_pPlayer->GetPosition().y - m_pB2Body->GetPosition().y;

		float angle = (atan(x / y));
		float n = sqrt(pow(x, 2) + pow(y, 2));
		m_velocity = Vector2((x / n) * m_fSpeed, (y / n) * m_fSpeed);
	}
	else
	{
		m_velocity = { 0, 0 };
	}

	m_pB2Body->SetLinearVelocity(b2Vec2(m_velocity.x, m_velocity.y));

}

void
Collectable::Draw(Renderer& renderer)
{
	//m_pCollectionArea->Draw(renderer, m_position, .0f);
	m_pSprite->Draw(renderer, m_position, .0f);
}

#ifdef _DEBUG
void
Collectable::DebugDraw()
{
	//ImGui::Text("Player in range: %i", m_bIsPlayerInRange);
	//ImGui::Text("Player is full: %i", m_bIsPlayerFull);
	//ImGui::Text("Player size: %i", m_pPlayer->GetStorageCurrentSize());
}
#endif _DEBUG

void Collectable::OnCollision(UserData* userData)
{
	if (userData->m_type == ET_PLAYER && !m_bIsCollected && !m_pPlayer->GetIsPlayerFull())
	{
		m_bIsPlayerFull = false;
		m_pPlayer->AddCollectable(this);
		m_bIsHeld = true;
		m_bIsToBeDeleted = true;
		m_pSprite->SetAlpha(0);
		m_pCollectionArea->SetAlpha(0);
		SoundManager::GetInstance().playSound(m_pCoinSound, 0, false, 0);
	}
}

void Collectable::OnCollisionExit(UserData* userData)
{
}

void Collectable::OnCollisionSensor(UserData* userData)
{
	if (userData->m_type == ET_PLAYER)
	{
		m_bIsPlayerInRange = true;
	}
}

void Collectable::OnCollisionSensorExit(UserData* userData)
{
	if (userData->m_type == ET_PLAYER)
	{
		m_bIsPlayerInRange = false;
	}
}

bool
Collectable::GetIsCollected()
{
	return m_bIsCollected;
}

bool Collectable::GetIsHeld()
{
	return m_bIsHeld;
}

bool Collectable::GetIsPlayerInRange()
{
	return m_bIsPlayerInRange;
}

void Collectable::SetCollected()
{
	m_bIsCollected = true;
}

b2Body * Collectable::GetBody()
{
	return m_pB2Body;
}

