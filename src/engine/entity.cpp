#include "entity.h"

#include "renderer.h"
#include "spritesheetanimation.h"
#include "logmanager.h"
#include <cstdio>
#include <limits>
#include <utils.h>
#include <fmod.hpp>

Entity::Entity()
	: m_position{ 0, 0 }
	, m_velocity{ 0, 0 }
	, m_bInitalised(false)
	, m_bIsLoaded(false)
	, m_bIsToBeDeleted(false)
	, m_fAngle(0)
	, m_fSpeed(1)
{
	m_id = ID();
	LogManager::GetInstance().Log("Entity Generated with ID: " + m_id.AsString());
}

Entity::~Entity()
{

}

bool 
Entity::Initialise(Renderer& renderer, FMOD::System& fmodSystem)
{
	if (m_bInitalised)
	{
		std::string message = "Attemped to re-initalise entity {ID:" + m_id.AsString() + "}";
		LogManager::GetInstance().Error(message);
		return false;
	}

	m_pFMODsystem = &fmodSystem;
	return true;
}

void Entity::SetPosition(Vector2 position)
{
	m_pB2Body->SetTransform(b2Vec2(position.x, position.y), m_pB2Body->GetAngle());
}

bool
Entity::IsColliding(Vector2 position, float radius)
{
	Vector2 diff = m_position - position;
	float distance = diff.Length();

	return distance < m_fCollisionRadius + radius;
}

bool
Entity::IsLoaded() const
{
	return m_bIsLoaded;
}

void 
Entity::Unload()
{
	m_bIsLoaded = false;
}

Vector2
Entity::GetPosition()
{
	return m_position;
}

Vector2
Entity::GetVelocity()
{
	return m_velocity;
}

ID
Entity::GetId() const
{
	return m_id;
}

std::string
Entity::GetIdAsString()
{
	return m_id.AsString();
}

void Entity::OnCollision(UserData* type)
{
}

void Entity::OnCollisionExit(UserData* type)
{
}

float&
Entity::GetCollisionRadius()
{
	return m_fCollisionRadius;
}

float Entity::GetAngle()
{
	return m_fAngle;
}

EntityType Entity::GetBodyEntityType()
{
	return m_pUserData->m_type;
}

bool Entity::GetIsToBeDeleted()
{
	return m_bIsToBeDeleted;
}

void Entity::SetIsToBeDeleted(bool state)
{
	m_bIsToBeDeleted = state;
}


