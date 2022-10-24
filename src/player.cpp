#include "player.h"

#include <engine/renderer.h>
#include <engine/sprite.h>
#include <engine/spritesheetanimation.h>
#include <engine/input.h>
#include <engine/xinputcontroller.h>
#include <engine/camera.h>
#include <engine/entity.h>
#include <engine/scenemanager.h>
#include <engine/soundmanager.h>
#include <logmanager.h>
#include <box2d/box2d.h>

#include <engine/scene.h>

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

Player::Player()
	: m_fSneakSpeed(.3)
	, m_fLeftStickDeadzone(0.2)
	, m_fRighttickDeadzone(0.8)
	, m_pPlayerAbilities(0)
	, m_bDoOnCaught(false)
	, m_iPlayerHP(3)
{
}

Player::~Player()
{
	delete m_pSprite;
	m_pSprite = 0;

	delete m_pDebugSprite;
	m_pDebugSprite = 0;

	delete m_pUserData;
	m_pUserData = 0;

	delete m_pPlayerStorage;
	m_pPlayerStorage = 0;

	delete m_pPlayerAbilities;
	m_pPlayerAbilities = 0;
}

bool
Player::Initialise(Renderer& renderer)
{
	if (!Entity::Initialise(renderer, SoundManager::GetInstance()))
	{
		return false;
	}

	m_fSpeed = 10;

	m_pSprite = renderer.CreateSprite("assets\\sprites\\entities\\ball.png", SpriteOrigin::CENTER);
	m_pDebugSprite = renderer.CreateSprite("assets\\sprites\\entities\\bullet.png", SpriteOrigin::CENTER);

	// B2Body init
	m_pUserData = new UserData;
	m_pUserData->m_pEntityPointer = this;
	m_pUserData->m_type = ET_PLAYER;

	//Body def
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(m_position.x, m_position.y);
	bodyDef.userData.pointer = (uintptr_t)m_pUserData;
	m_pB2Body = SceneManager::GetInstance().m_pWorld->CreateBody(&bodyDef);

	
	//Fixture def
	b2CircleShape dynamicCircle;
	dynamicCircle.m_radius = Renderer::ScreenSpaceToWorldSpace(m_pSprite->GetHeight() / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicCircle;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.userData.pointer = (uintptr_t)m_pUserData;
	fixtureDef.filter.categoryBits = CATEGORY_PLAYER;
	fixtureDef.filter.maskBits = MASK_PLAYER;

	m_pB2Fixture = m_pB2Body->CreateFixture(&fixtureDef);
	
	m_pPlayerStorage = new PlayerStorage;

	m_pPlayerAbilities = new AbilityManager;
	m_pPlayerAbilities->Initialise(renderer, this);

	// Sound
	SoundManager::GetInstance().createSound("assets\\sounds\\player\\death.mp3", FMOD_DEFAULT, 0, &m_pDeathSound);

	return true;
}

void Player::Setup(Vector2 safeZoneCenter)
{
	m_safeZoneCenter = safeZoneCenter;
}

void
Player::Process(float deltaTime, Input& input)
{
	m_pController = input.GetController(0);

	// Controller Inputs
	if (input.GetNumberOfControllersAttached() > 0)
	{
		m_movementInput = m_pController->GetLeftStick() / std::numeric_limits<int16_t>::max();
		m_rotationInput = m_pController->GetRightStick() / std::numeric_limits<int16_t>::max();

		float rightTrigger = m_pController->GetRightTrigger() / std::numeric_limits<int16_t>::max();
		float leftTrigger = m_pController->GetLeftTrigger() / std::numeric_limits<int16_t>::max();
	}

	// Aim
	if (m_rotationInput.LengthSquared() > powf(0.8f, 2))
	{
		float angleToLerpTo = -atan2f(m_rotationInput.y, m_rotationInput.x) - static_cast<float>(M_PI) / 2.f;
		if (abs(m_fAngle - angleToLerpTo) > static_cast<float>(M_PI) * 1.5f && deltaTime > 0.f)
		{
			m_fAngle = angleToLerpTo;
		}
		else
		{
			m_fAngle = Lerp(m_fAngle, angleToLerpTo, 50.f * deltaTime);
		}

	}

	ProcessMovement();
	m_pPlayerAbilities->Process(deltaTime, input);

	if (m_bDoOnCaught)
	{
		m_bDoOnCaught = false;
		OnCaught();
	}
}

void
Player::Draw(Renderer& renderer)
{
	m_pSprite->Draw(renderer, m_position, m_fAngle);
	m_pDebugSprite->Draw(renderer, Vector2((m_pB2Body->GetPosition().x), (m_pB2Body->GetPosition().y)), 0.0f);
	m_pPlayerAbilities->Draw(renderer);
}

#ifdef _DEBUG
void
Player::DebugDraw()
{
	ImGui::Text("Player x: %f", m_pB2Body->GetPosition().x);
	ImGui::Text("Player y: %f", m_pB2Body->GetPosition().y);
	m_pPlayerAbilities->DebugDraw();
}
#endif _DEBUG

void Player::OnCollision(UserData* userData)
{
	LogManager::GetInstance().Log("Player collision method ");
	switch (userData->m_type)
	{
	case ET_ENEMY:
		m_bDoOnCaught = true;
		m_iPlayerHP--;
		SoundManager::GetInstance().playSound(m_pDeathSound, 0, false, 0);
		break;
	}
}

PlayerStorage* Player::GetPlayerStorage()
{
	return m_pPlayerStorage;
}

bool Player::GetIsPlayerFull()
{
	return m_pPlayerStorage->GetIsFull();
}

int Player::GetStorageCurrentSize()
{
	return m_pPlayerStorage->GetCurrentSize();
}

void Player::AddCollectable(Collectable * collectable)
{
	m_pPlayerStorage->AddItem(collectable);
}

void Player::OnCaught()
{
	SetPosition(m_safeZoneCenter);
}

bool Player::GetIsSneaking()
{
	return m_isSneaking;
}

int Player::GetPlayerHP()
{
	return m_iPlayerHP;
}

bool Player::IsPlayerDead()
{
	return m_iPlayerHP <= 0;
}

AbilityManager* 
Player::GetAbilityManager()
{
	return m_pPlayerAbilities;
}

void
Player::ProcessMovement()
{
	// Movement
	if (m_movementInput.LengthSquared() > powf(m_fLeftStickDeadzone, 2))
	{
		m_velocity = { m_movementInput.x * m_fSpeed, -m_movementInput.y * m_fSpeed };
	}
	else
	{
		m_velocity = Vector2(0, 0);
	}

	// TODO: Change to right trigger
	//if (m_pController->GetButtonState(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == BS_HELD)
	//{
	//	m_velocity *= m_fSneakSpeed;
	//	m_isSneaking = true;
	//}
	//else
	//{
		m_isSneaking = false;
	//}

	m_pB2Body->SetLinearVelocity(b2Vec2(m_velocity.x, m_velocity.y));

	m_position.x = m_pB2Body->GetWorldCenter().x;
	m_position.y = m_pB2Body->GetWorldCenter().y;
}