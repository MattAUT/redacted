#include "abilityflashlight.h"

#include <engine/renderer.h>
#include <engine/sprite.h>
#include <imgui/imgui_impl_sdl.h>
#include <logmanager.h>
#include <engine/entity.h>
#include <box2d/box2d.h>
#include <engine/scenemanager.h>
#include <utils/utils.h>
#include <engine/entity.h>
#include <vector>
#include <engine/id.h>

// Sound
#include <engine/soundmanager.h>
#include <fmod.hpp>
#include <game/enemy.h>

#include "player.h"
#include "raycastquery.h"

AbilityFlashLight::AbilityFlashLight()
	: m_bIsHeld(false)
	, m_bSeesEnemy(false)
	, m_bCheck(false)
	, m_fEnemyCount(0)
	, m_fStunDuration(3)
{
	m_fCoolDown = 1;
	m_fDuration = .2;
	m_fCoolDownTimer = m_fCoolDown;
}

AbilityFlashLight::~AbilityFlashLight()
{
}

void
AbilityFlashLight::Process(float deltaTime, Input& input)
{
	if (m_fCoolDownTimer >= m_fCoolDown)
	{
		if (input.GetController(0)->GetButtonState(m_button) == BS_HELD)
		{
			m_bIsHeld = true;
		}
		if (input.GetController(0)->GetButtonState(m_button) == BS_RELEASED)
		{
			SoundManager::GetInstance().playSound(m_pFlashSound, 0, false, 0);
			m_bIsHeld = false;
			m_fCoolDownTimer = 0;
			m_bIsActive = true;
			m_position = m_pPlayer->GetPosition();
			m_fAngle = m_pPlayer->GetAngle();

			LogManager::GetInstance().Log("--------------Start------------");
			for (Enemy* enemy : m_mEnemies)
			{
				m_bCheck = CheckIfEnemiesAreInLOS(enemy);
			}
			LogManager::GetInstance().Log("-------------End------------");
		}
	}

	if (m_bIsActive)
	{
		m_fDurationCounter += deltaTime;
		m_pFlash->SetAlpha(1 - (m_fDurationCounter / m_fDuration));
	}

	if (m_fDurationCounter >= m_fDuration)
	{
		m_bIsActive = false;
		m_fDurationCounter = 0;
		m_pFlash->SetAlpha(0.5);
	}

	m_fCoolDownTimer = Clamp(0, m_fCoolDownTimer + deltaTime, m_fCoolDown);
	float angle = m_pPlayer->GetAngle() + static_cast<float>(M_PI) / 2.f;
	m_direction = Vector2(cos(angle), sin(angle));

	m_box2dPos = { m_pPlayer->GetPosition().x + (m_direction.x * m_distance), m_pPlayer->GetPosition().y + (m_direction.y * m_distance) };
	m_pB2Body->SetTransform(b2Vec2(m_box2dPos.x, m_box2dPos.y), m_pPlayer->GetAngle());
}

void
AbilityFlashLight::UseAbility(Vector2 pos)
{
}

bool
AbilityFlashLight::Initialise(Renderer& renderer, Player & player, SDL_GameControllerButton button)
{
	if (!Entity::Initialise(renderer, SoundManager::GetInstance()))
	{
		return false;
	}

	m_pB2World = SceneManager::GetInstance().m_pWorld;

	m_pPlayer = &player;
	m_button = button;
	int scale = 4;

	m_sName = "Flash Light";

	m_pUserData = new UserData;
	m_pUserData->m_pEntityPointer = this;
	m_pUserData->m_type = ET_FLASHLIGHT;

	m_pFlash = renderer.CreateSprite("assets\\sprites\\entities\\TriangleFull.png", SpriteOrigin::BOTTOM_CENTER);
	m_pFlash->SetScale(scale);
	m_pFlash->SetAlpha(.5);

	m_distance = renderer.ScreenSpaceToWorldSpace(m_pFlash->GetHeight() / 2);

	m_pSprite = renderer.CreateSprite("assets\\sprites\\entities\\Triangle.png", SpriteOrigin::BOTTOM_CENTER);
	m_pSprite->SetScale(scale);

	m_pDebugSprite = renderer.CreateSprite("assets\\sprites\\entities\\Triangle.png", SpriteOrigin::CENTER);
	m_pDebugSprite->SetScale(scale);
	m_pDebugSprite->SetGreenTint(0);
	//Body def
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(m_position.x, m_position.y);
	bodyDef.userData.pointer = (uintptr_t)m_pUserData;
	m_pB2Body = SceneManager::GetInstance().m_pWorld->CreateBody(&bodyDef);

	//Fixture def
	b2PolygonShape flashConeShape;
	flashConeShape.SetAsBox(Renderer::ScreenSpaceToWorldSpace(m_pFlash->GetWidth() / 2), (Renderer::ScreenSpaceToWorldSpace(m_pFlash->GetHeight() / 2)));

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &flashConeShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.userData.pointer = (uintptr_t)m_pUserData;
	fixtureDef.isSensor = (true);
	fixtureDef.filter.categoryBits = CATEGORY_ABILITY;
	fixtureDef.filter.maskBits = MASK_ABILITY;

	m_pB2Fixture = m_pB2Body->CreateFixture(&fixtureDef);

	// Sound
	SoundManager::GetInstance().createSound("assets\\sounds\\abilities\\flash.mp3", FMOD_DEFAULT, 0, &m_pFlashSound);

	return true;
}

void
AbilityFlashLight::Draw(Renderer & renderer)
{
	if (m_bIsDebuging)
	{
		m_pDebugSprite->Draw(renderer, Vector2(m_pB2Body->GetPosition().x, m_pB2Body->GetPosition().y), m_pPlayer->GetAngle());
	}

	if (m_bIsHeld)
	{
		m_pSprite->Draw(renderer, m_pPlayer->GetPosition(), m_pPlayer->GetAngle());
	}
	if (m_bIsActive) //m_bIsActive
	{
		m_pFlash->Draw(renderer, m_position, m_fAngle);
	}
}

void
AbilityFlashLight::DebugDraw()
{
#ifdef _DEBUG
	Ability::DebugDraw();
	ImGui::Text("Check: %i", m_bCheck);
	ImGui::Text("Enemy Map Count: %i", m_mEnemies.size());
	ImGui::Text("Enemy Counter: %i", m_fEnemyCount);
	ImGui::Text("--- Enemy ID List ---");

	for (Enemy* enemy : m_mEnemies)
	{
		ImGui::Text("Enemy* ID");
		ImGui::Text(enemy->GetIdAsString().c_str());
	}
#endif _DEBUG
}

void
AbilityFlashLight::OnCollision(UserData* userData)
{
	if (userData->m_type == ET_ENEMY)
	{
		m_bSeesEnemy = true;
		m_mEnemies.push_back(reinterpret_cast<Enemy*>(userData->m_pEntityPointer));
		m_fEnemyCount++;
	}
}

void
AbilityFlashLight::OnCollisionExit(UserData* userData)
{
	if (userData->m_type == ET_ENEMY)
	{
		ID id = reinterpret_cast<Enemy*>(userData->m_pEntityPointer)->GetId();

		for (int i = 0; i < m_mEnemies.size(); i++)
		{
			if (m_mEnemies[i]->GetId() == id)
			{
				m_mEnemies.erase(m_mEnemies.begin() + (i));
				break;
			}
		}

		m_fEnemyCount--;

		if (m_fEnemyCount == 0)
		{
			m_bSeesEnemy = false;
		}
	}
}

bool
AbilityFlashLight::CheckIfEnemiesAreInLOS(Enemy* enemy)
{
	RaycastQuery callback;

	Vector2 p1 = m_position;
	Vector2 p2 = enemy->GetPosition();

	m_pB2World->RayCast(&callback, p1.ToB2Vec(), p2.ToB2Vec());

	UserData* userData = reinterpret_cast<UserData*>(callback.out_fixture->GetUserData().pointer);

	if (userData != nullptr)
	{
		LogManager::GetInstance().Log(EntityTypeToString(userData->m_type));

		if (userData->m_type == ET_ENEMY)
		{
			reinterpret_cast<Enemy*>(userData->m_pEntityPointer)->Stun(m_fStunDuration);
		}
	}

	return true;
}
