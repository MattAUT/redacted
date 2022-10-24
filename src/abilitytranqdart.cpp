#include "abilitytranqdart.h"

#include <engine/renderer.h>
#include <engine/sprite.h>
#include <imgui/imgui_impl_sdl.h>
#include <logmanager.h>
#include <engine/entity.h>
#include <box2d/box2d.h>
#include <engine/scenemanager.h>
#include <utils/utils.h>
#include <engine/entity.h>

// Sound
#include <engine/soundmanager.h>
#include <fmod.hpp>

#include "player.h"


AbilityTranqDart::AbilityTranqDart()
	:m_fSpeed(20)
	, m_direction(0, 0)
{
	m_fCoolDown = 10;
	m_fCoolDownTimer = m_fCoolDown;
	m_fDuration = 2;
}

AbilityTranqDart::~AbilityTranqDart()
{

}

void AbilityTranqDart::UseAbility(Vector2 pos)
{
}

bool
AbilityTranqDart::Initialise(Renderer & renderer, Player & player, SDL_GameControllerButton button)
{
	if (!Entity::Initialise(renderer, SoundManager::GetInstance()))
	{
		return false;
	}

	m_pPlayer = &player;
	m_button = button;

	m_pUserData = new UserData;
	m_pUserData->m_pEntityPointer = this;
	m_pUserData->m_type = ET_TRANQDART;

	m_sName = "Tranq Dart";
	m_pSprite = renderer.CreateSprite("assets\\sprites\\entities\\dart.png", SpriteOrigin::CENTER);
	m_pDebugSprite = renderer.CreateSprite("assets\\sprites\\entities\\bullet.png", SpriteOrigin::CENTER);
	m_pSprite->SetRedTint(1.0f);
	m_pSprite->SetGreenTint(0.1f);
	m_pSprite->SetBlueTint(0.1f);
	m_pSprite->SetAlpha(1.0f);
	m_pSprite->SetScale(1);


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

	fixtureDef.filter.categoryBits = CATEGORY_ABILITY;
	fixtureDef.filter.maskBits = MASK_ABILITY;


	m_pB2Fixture = m_pB2Body->CreateFixture(&fixtureDef);

	// Sound
	SoundManager::GetInstance().createSound("assets\\sounds\\abilities\\dart.mp3", FMOD_DEFAULT, 0, &m_pDartSound);
	SoundManager::GetInstance().createSound("assets\\sounds\\abilities\\jump.mp3", FMOD_DEFAULT, 0, &m_pJumpSound);

	return true;
}

void AbilityTranqDart::Draw(Renderer & renderer)
{
	if (m_bIsActive)
	{
		m_pSprite->Draw(renderer, m_position, m_fDartAngle - static_cast<float>(M_PI) / 2.f);
	}
}

void AbilityTranqDart::Process(float deltaTime, Input & input)
{
	m_position = Vector2(m_pB2Body->GetPosition().x, m_pB2Body->GetPosition().y);


	if (input.GetController(0)->GetButtonState(m_button) == BS_PRESSED)
	{
		if (m_fCoolDownTimer >= m_fCoolDown)
		{
			if (!m_bIsActive)
			{
				SoundManager::GetInstance().playSound(m_pDartSound, 0, false, 0);
				m_position = m_pPlayer->GetPosition();
				m_bIsActive = true;
				LogManager::GetInstance().Log("Tranq Used");
				m_fDartAngle = m_pPlayer->GetAngle() + static_cast<float>(M_PI) / 2.f;
				m_direction = Vector2(cos(m_fDartAngle), sin(m_fDartAngle));
				m_fCoolDownTimer = 0;
			}
		}
		else {
			if (m_bIsActive)
			{
				SoundManager::GetInstance().playSound(m_pJumpSound, 0, false, 0);
				m_fDartAngle = m_pPlayer->GetAngle() + static_cast<float>(M_PI) / 2.f;
				m_direction = Vector2(cos(m_fDartAngle), sin(m_fDartAngle));

			}
		}
	}

	if (m_bIsActive)
	{
		m_pB2Body->SetLinearVelocity(b2Vec2(m_direction.x * m_fSpeed, m_direction.y * m_fSpeed));
		m_fDurationCounter += deltaTime;
		if (m_fDurationCounter >= m_fDuration)
		{
			m_bIsActive = false;
			m_fDurationCounter = 0;
		}
	}
	else
	{
		m_pB2Body->SetTransform(b2Vec2(m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y), .0f);
		m_position = m_pPlayer->GetPosition();
		m_fCoolDownTimer = Clamp(0, m_fCoolDownTimer + deltaTime, m_fCoolDown);
	}
}

void AbilityTranqDart::DebugDraw()
{
#ifdef _DEBUG
	Ability::DebugDraw();
#endif _DEBUG
}

void AbilityTranqDart::OnCollision(UserData* userData)
{
	//LogManager::GetInstance().Log("Tranq Collding with:");
	//LogManager::GetInstance().Log(EntityTypeToString(type));
	m_bIsActive = false;
	m_fDurationCounter = 0;
}


