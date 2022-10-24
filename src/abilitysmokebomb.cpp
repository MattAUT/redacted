#include "abilitysmokebomb.h"

#include <engine/renderer.h>
#include <engine/sprite.h>
#include <imgui/imgui_impl_sdl.h>
#include <logmanager.h>
#include <engine/entity.h>
#include <box2d/box2d.h>
#include <engine/scenemanager.h>
#include <engine/entity.h>
#include <engine/xinputcontroller.h>
#include <engine/particleemitter.h>

// Sound
#include <engine/soundmanager.h>
#include <fmod.hpp>

#include "player.h"

AbilitySmokeBomb::AbilitySmokeBomb()
{
	m_fCoolDown = 3;
	m_fCoolDownTimer = m_fCoolDown;
	m_fDuration = 3;
}

AbilitySmokeBomb::~AbilitySmokeBomb()
{
}

void AbilitySmokeBomb::Process(float deltaTime, Input& input)
{
	if (m_fCoolDownTimer >= m_fCoolDown)
	{
		if (input.GetController(0)->GetButtonState(m_button) == BS_PRESSED)
		{
			SoundManager::GetInstance().playSound(m_pSmokeSound, 0, false, 0);
			m_position = m_pPlayer->GetPosition();
			m_pB2Body->SetTransform(m_position.ToB2Vec(), 0.f);
			m_bIsActive = true;
			LogManager::GetInstance().Log("SmokeBomb Used");
			m_pParticleEmitter->SetPosition(m_position);
			m_pParticleEmitter->SetActive(true);
			//m_pParticleEmitter->SetParticleLifeSpan(2);
		}
	}

	if (m_bIsActive)
	{ //(m_fDurationCounter / m_fDuration)
		float a = 2.8; //amplitude
		float b = 4.9; 
		float c = 2;
		float d = 1.9;
		float f = 5.2; //Frequency
		m_pParticleEmitter->SetParticleLifeSpan(c - (a * sin(pow((d * (m_fDurationCounter) / b),f))));
		m_fDurationCounter += deltaTime;
		if (m_fDurationCounter >= m_fDuration)
		{
			m_bIsActive = false;
			m_fDurationCounter = 0;
			m_pParticleEmitter->SetActive(false);
		}
		m_fCoolDownTimer = 0;
		m_pSprite->SetAlpha(1 - (m_fDurationCounter / m_fDuration));
	}
	else
	{
		m_fCoolDownTimer = Clamp(0, m_fCoolDownTimer + deltaTime, m_fCoolDown);
	}

	m_pParticleEmitter->Process(deltaTime);

}

void AbilitySmokeBomb::UseAbility(Vector2 pos)
{

}

bool AbilitySmokeBomb::Initialise(Renderer& renderer, Player & player, SDL_GameControllerButton button)
{
	if (!Entity::Initialise(renderer, SoundManager::GetInstance()))
	{
		return false;
	}

	m_sName = "Smoke Bomb";

	m_pPlayer = &player;
	m_button = button;

	m_pParticleSprite = renderer.CreateSprite("assets\\sprites\\entities\\ball.png", SpriteOrigin::CENTER);
	m_pParticleEmitter = new ParticleEmitter;
	m_pParticleEmitter->Initialise(renderer);
	m_pParticleSprite->SetScale(.5);
	m_pParticleEmitter->SetBatchSize(50);
	m_pParticleEmitter->SetAccelerationScalar(.1);
	m_pParticleEmitter->SetEmissionRate(0.03);
	m_pParticleEmitter->SetParticleLifeSpan(2);
	m_pParticleEmitter->SetColour(.2f, .2f, .2f);

	m_pParticleEmitter->SetSharedSprite(m_pParticleSprite);

	m_pDebugSprite = renderer.CreateSprite("assets\\sprites\\entities\\ball.png", SpriteOrigin::CENTER);
	m_pDebugSprite->SetAlpha(0.5);
	m_pDebugSprite->SetRedTint(.5f);
	m_pDebugSprite->SetGreenTint(.5f);
	m_pDebugSprite->SetBlueTint(.5f);
	m_pDebugSprite->SetScale(5);

	m_pSprite = renderer.CreateSprite("assets\\sprites\\entities\\cloud.png", SpriteOrigin::CENTER);
	m_pSprite->SetRedTint(.1f);
	m_pSprite->SetGreenTint(.1f);
	m_pSprite->SetBlueTint(.1f);
	m_pSprite->SetAlpha(0.9f);
	m_pSprite->SetScale(3);

	// b2body init
	m_pUserData = new UserData;
	m_pUserData->m_pEntityPointer = this;
	m_pUserData->m_type = ET_SMOKEBOMB;

	//body def
	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.position.Set(m_position.x, m_position.y);
	bodydef.userData.pointer = (uintptr_t)m_pUserData;
	bodydef.gravityScale = 0;
	m_pB2Body = SceneManager::GetInstance().m_pWorld->CreateBody(&bodydef);

	//fixture def sensor
	b2CircleShape dynamiccircle2;
	dynamiccircle2.m_radius = renderer.ScreenSpaceToWorldSpace(m_pDebugSprite->GetHeight() / 2);

	b2FixtureDef fixturedef2;
	fixturedef2.shape = &dynamiccircle2;
	fixturedef2.isSensor = true;
	fixturedef2.userData.pointer = (uintptr_t)m_pUserData;
	fixturedef2.filter.categoryBits = CATEGORY_ABILITY;
	fixturedef2.filter.maskBits = MASK_ABILITY;

	m_pB2Fixture = m_pB2Body->CreateFixture(&fixturedef2);

	// Sound
	SoundManager::GetInstance().createSound("assets\\sounds\\abilities\\smoke.mp3", FMOD_DEFAULT, 0, &m_pSmokeSound);

	return true;
}

void AbilitySmokeBomb::Draw(Renderer & renderer)
{
	m_pParticleEmitter->Draw(renderer);
	if (m_bIsActive)
	{
		m_pSprite->Draw(renderer, m_position, .0f);
		if (m_bIsDebuging)
		{
			m_pDebugSprite->Draw(renderer, Vector2::FromB2Vec(m_pB2Body->GetPosition()), .0f);
		}
	}
}

void AbilitySmokeBomb::DebugDraw()
{
#ifdef _DEBUG
	Ability::DebugDraw();
	m_pParticleEmitter->DebugDraw();
#endif _DEBUG
}

