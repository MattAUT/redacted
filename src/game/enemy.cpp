#include "enemy.h"

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
#include <path_finder.h>
#include <point.h>

#ifdef _DEBUG
#include <imgui/imgui_impl_sdl.h>
#endif // _DEBUG

#include <utils.h>
#include <limits>
#include <fmod.hpp>
#include "vector2.h"
#include "player.h";
#include "raycastcallback.h"

Enemy::Enemy()
	: m_fRunSpeedFactor(7.5f)
	, m_fSearchAngle(static_cast<float>(M_PI) / 6.f)
	, m_fSearchDistance(8.f)
	, m_fWaitTime(4.f)
	, m_fUntilSpotTime(0.f)
	, m_fUntilSpottedDuration(2.f)
	, m_fPatrolReturnTime(0.f)
	, m_fPatrolReturnDuration(4.f)
	, m_fStunTime(0.f)
	, m_fBlindDuration(3.5f)
	, m_state(ENEMY::PATROLLING)
	, m_bBlind(false)
{
}

Enemy::~Enemy()
{
	delete m_pSprite;
	m_pSprite = 0;

	delete m_pDebugSprite;
	m_pDebugSprite = 0;

	delete m_pUserData;
	m_pUserData = 0;

	delete m_pVisonConeSprite;
	m_pVisonConeSprite = 0;
}

bool
Enemy::Initialise(Renderer& renderer)
{
	if (!Entity::Initialise(renderer, SoundManager::GetInstance()))
	{
		return false;
	}

	m_fSpeed = 1;

	m_pSprite = renderer.CreateSprite("assets\\sprites\\entities\\ball.png", SpriteOrigin::CENTER);
	m_pSprite->SetGreenTint(0);
	m_pSprite->SetBlueTint(0);
	m_pDebugSprite = renderer.CreateSprite("assets\\sprites\\entities\\bullet.png", SpriteOrigin::CENTER);
	m_pVisonConeSprite = renderer.CreateSprite("assets\\sprites\\entities\\cone.png", SpriteOrigin::BOTTOM_CENTER);
	m_pVisonConeSprite->SetScale(4.f);

	m_fCollisionRadius = Renderer::ScreenSpaceToWorldSpace(m_pSprite->GetHeight() / 2);

	// B2Body init
	m_pUserData = new UserData;
	m_pUserData->m_pEntityPointer = this;
	m_pUserData->m_type = ET_ENEMY;

	//Body def
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(m_position.x, m_position.y);
	bodyDef.userData.pointer = (uintptr_t)m_pUserData;
	m_pB2Body = SceneManager::GetInstance().m_pWorld->CreateBody(&bodyDef);


	//Fixture def
	b2CircleShape dynamicCircle;
	dynamicCircle.m_radius = m_fCollisionRadius;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicCircle;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.userData.pointer = (uintptr_t)m_pUserData;
	fixtureDef.filter.categoryBits = CATEGORY_ENEMY;
	fixtureDef.filter.maskBits = MASK_ENEMY;

	m_pB2Fixture = m_pB2Body->CreateFixture(&fixtureDef);
}

void Enemy::Setup(Player& player, PathNode* nodes, b2World& world, NavMesh::PathFinder& pathFinder)
{
	m_pPlayer = &player;
	m_pPathNodes = nodes;
	m_pCurrentNode = &m_pPathNodes[1];
	m_pB2World = &world;
	m_pPathFinder = &pathFinder;
}

void
Enemy::Process(float deltaTime, Input& input)
{
	switch (m_state)
	{
	case ENEMY::PATROLLING:
		PatrolProcess(deltaTime);
		break;
	case ENEMY::CHASING:
		ChaseProcess(deltaTime);
		break;
	case ENEMY::STUNNED:
		StunProcess(deltaTime);
		break;
	}

	m_bPlayerInLOS = IsPlayerInLOS();
	m_fAngleToPlayer = -atan2(m_position.y - m_pPlayer->GetPosition().y, m_pPlayer->GetPosition().x - m_position.x);

	static Vector2 previousTarget;
	if (!m_bPlayerInLOS && !(previousTarget == m_targetPosition))
	{
		std::vector<NavMesh::Point> points = { m_position.ToNavMeshPoint(), m_targetPosition.ToNavMeshPoint() };
		m_pPathFinder->AddExternalPoints(points);
		m_pathedPositionTargets = m_pPathFinder->GetPath(points[0], points[1]);
	}

	previousTarget = m_targetPosition;

	if (m_pathedPositionTargets.size() > 1)
	{
		m_positionToMoveTo = m_pathedPositionTargets[0];
		if (IsColliding(Vector2::FromNavMeshPoint(m_positionToMoveTo), -(m_fCollisionRadius / 2.f)))
		{
			m_positionToMoveTo = m_pathedPositionTargets[1];
			m_pathedPositionTargets.erase(m_pathedPositionTargets.begin());
		}
	}
	else
	{
		m_positionToMoveTo = m_targetPosition.ToNavMeshPoint();
	}

	float angleToLerpTo = -atan2(m_position.y - m_positionToMoveTo.y, m_positionToMoveTo.x - m_position.x);
	if (abs(m_fAngle - angleToLerpTo) > static_cast<float>(M_PI) * 1.5f && deltaTime > 0.f)
	{
		m_fAngle = angleToLerpTo;
	}
	else
	{
		m_fAngle = Lerp(m_fAngle, angleToLerpTo, 10.f * deltaTime);
	}

	m_velocity = Vector2(m_fAngle, m_fSpeed * (m_state == ENEMY::CHASING ? m_fRunSpeedFactor : 1.f), POLAR);
	if (m_bFreeze)
	{
		m_pB2Body->SetLinearVelocity({ 0.f, 0.f });
	}
	else
	{
		m_pB2Body->SetLinearVelocity(b2Vec2(m_velocity.x, m_velocity.y));
	}

	m_position = Vector2::FromB2Vec(m_pB2Body->GetWorldCenter());
}

void
Enemy::PatrolProcess(float deltaTime)
{
	m_bFreeze = false;
	if (m_fAngleToPlayer < m_fAngle + m_fSearchAngle &&
		m_fAngleToPlayer > m_fAngle - m_fSearchAngle)
	{
		if (m_bBlind)
		{
			m_fBlindTime += deltaTime;

			if (m_fBlindTime > m_fBlindDuration)
			{
				m_bBlind = false;
			}

			return;
		}

		if (m_bPlayerInLOS)
		{
			Vector2 diff = m_position - m_pPlayer->GetPosition();
			float distance = diff.Length();
			if (distance < m_fSearchDistance) {
				m_bFreeze = true;
				m_fUntilSpotTime += deltaTime;

				if (m_fUntilSpotTime > m_fUntilSpottedDuration)
				{
					m_state = ENEMY::CHASING;
					m_bFreeze = false;
				}

				return;
			}
		}
	}

	m_fUntilSpotTime = 0.f;
	m_fBlindTime = 0.f;

	m_targetPosition = m_pCurrentNode->position;
	if (IsColliding(m_pCurrentNode->position, 0.f))
	{
		m_pCurrentNode = m_pCurrentNode->nextNode;
	}
}

void Enemy::ChaseProcess(float deltaTime)
{
	if (m_bPlayerInLOS)
	{
		m_targetPosition = m_pPlayer->GetPosition();
	}

	m_bFreeze = false;
	if (IsColliding(m_targetPosition, -m_fCollisionRadius / 2.f))
	{
		m_fPatrolReturnTime += deltaTime;
		m_bFreeze = true;

		if (m_fPatrolReturnTime > m_fPatrolReturnDuration)
		{
			m_state = ENEMY::PATROLLING;
		}

		return;
	}

	m_fPatrolReturnTime = 0.f;
}

void Enemy::StunProcess(float deltaTime)
{
	m_bFreeze = true;
	m_fStunTime += deltaTime;

	if (m_fStunTime > m_fStunDuration)
	{
		m_state = m_previousState;
		m_fStunTime = 0.f;
	}

	float t = m_fStunTime / m_fStunDuration;
	float visionConeAlpha = powf(sinf(1600 * powf((t-0.5), 10)), 20);
	m_pVisonConeSprite->SetAlpha(visionConeAlpha);
}

void Enemy::Stun(float duration)
{
	m_previousState = m_state != ENEMY::STUNNED ? m_state : m_previousState;
	m_state = ENEMY::STUNNED;
	m_fStunDuration = duration;

	std::string t = std::to_string(duration);
	char const *n_char = t.c_str();

	LogManager::GetInstance().Log(n_char);
}

void
Enemy::Draw(Renderer& renderer)
{
	m_pVisonConeSprite->Draw(renderer, m_position, m_fAngle - static_cast<float>(M_PI) / 2.f);
	m_pSprite->Draw(renderer, m_position, m_fAngle);
	m_pDebugSprite->Draw(renderer, Vector2((m_pB2Body->GetPosition().x), (m_pB2Body->GetPosition().y)), 0.0f);

#ifdef _DEBUG
	m_pDebugSprite->Draw(renderer, dm_Raycastp1, 0.0f);
	m_pDebugSprite->Draw(renderer, dm_Raycastp2, 0.0f);

	for (auto navpoint = m_pathedPositionTargets.begin(); navpoint != m_pathedPositionTargets.end(); ++navpoint)
	{
		m_pDebugSprite->Draw(renderer, Vector2::FromNavMeshPoint(*navpoint), 0.0f);
	}
#endif //_DEBUG
}

// https://stackoverflow.com/a/5094430
inline const char* EnemyStateToString(ENEMY::State s)
{
	switch (s)
	{
	case ENEMY::PATROLLING:   return "Patrolling";
	case ENEMY::CHASING:   return "Chasing";
	case ENEMY::STUNNED:   return "Stunned";
	default:      return "[Unknown State]";
	}
}

#ifdef _DEBUG
void
Enemy::DebugDraw()
{
	ImGui::Text("Enemy: %s", m_id.AsString().c_str());
	ImGui::Text("State: %s", EnemyStateToString(m_state));
	ImGui::Text("Position: {%f, %f}", m_position.x, m_position.y);
	ImGui::Text("Velocity: {%f, %f}", m_velocity.x, m_velocity.y);
	ImGui::Text("Angle: {%f}", m_fAngle);
	ImGui::Text("Angle to Player: {%f}", m_fAngleToPlayer);
	ImGui::Text("Player in LOS: {%s}", m_bPlayerInLOS ? "true" : "false");
	ImGui::Text("Current Node Position: {%f, %f}", m_pCurrentNode->position.x, m_pCurrentNode->position.y);
	ImGui::Spacing();
	ImGui::Text("Movement Target: {%f, %f}", m_targetPosition.x, m_targetPosition.y);
	ImGui::Text("Attempted Movement Position: {%f, %f}", m_positionToMoveTo.x, m_positionToMoveTo.y);
}
#endif _DEBUG

void Enemy::OnCollision(UserData* userData)
{
	switch (userData->m_type)
	{
	case ET_PLAYER:
		m_state = ENEMY::PATROLLING;
		break;
	case ET_SMOKEBOMB:
		LogManager::GetInstance().Log("Collided with smoke bomb");
		m_bBlind = true;
		break;
	case ET_FLASHLIGHT:
		break;
	case ET_TRANQDART:
		LogManager::GetInstance().Log("Enemy got tranq'd");
		Stun(10);
		break;
	default:
		break;
	}
}

bool Enemy::IsPlayerInLOS()
{
	RaycastCallback callback;
	Vector2 offset = Vector2(m_fAngleToPlayer, m_fCollisionRadius + 0.2f, POLAR);
	Vector2 p1 = m_position + offset;
	Vector2 p2 = m_pPlayer->GetPosition() - offset;
#ifdef _DEBUG
	dm_Raycastp1 = p1;
	dm_Raycastp2 = p2;
#endif //_DEBUG
	m_pB2World->RayCast(&callback, p1.ToB2Vec(), p2.ToB2Vec());
	return !callback.bHit;
}

