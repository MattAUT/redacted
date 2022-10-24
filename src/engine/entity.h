#ifndef __ENTITY_H__
#define __ENTITY_H__

class Entity;
class Renderer;
class Input;
class Node;
class SpriteSheetAnimation;
class SmokeEmitter;
class b2Body;
class b2World;
class b2Fixture;

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

#include "id.h"
#include <vector2.h>
#include <string>
#include <tuple>

enum EntityType
{
	ET_WALL,
	//Entities
	ET_PLAYER,
	ET_ENEMY,
	ET_COLLECTABLE,
	ET_COLLECTSENSOR,
	ET_SAFEAREA,

	//Abilities
	ET_SMOKEBOMB,
	ET_FLASHLIGHT,
	ET_TRANQDART,
};

// https://stackoverflow.com/a/5094430
inline const char* EntityTypeToString(EntityType s)
{
	switch (s)
	{
	case ET_WALL:
		return("Wall");
		break;
	case ET_PLAYER:
		return("Player");
		break;
	case ET_ENEMY:
		return("Enemy");
		break;
	case ET_COLLECTABLE:
		return("Collecable");
		break;
	case ET_COLLECTSENSOR:
		return("CollSensor");
		break;
	case ET_SAFEAREA:
		return("SafeArea");
		break;
	case ET_SMOKEBOMB:
		return("SmokeBomb");
		break;
	case ET_FLASHLIGHT:
		return("FlashLight");
		break;
	case ET_TRANQDART:
		return("TranqDart");
		break;
	default:
		break;
	}
}

class UserData
{
public:
	Entity* m_pEntityPointer;
	EntityType m_type;
};

//Collision Filters
const short CATEGORY_PLAYER = 0x0001;
const short CATEGORY_ENEMY = 0x0002;
const short CATEGORY_COLLECTABLE = 0x0004;
const short CATEGORY_COLLECTSENSOR = 0x0008;
const short CATEGORY_SAFEAREA = 0x0010;
const short CATEGORY_ABILITY = 0x0020;
const short CATEGORY_WALLS = 0x0040;

//Collision Masks
const short MASK_PLAYER = ~CATEGORY_ABILITY;
const short MASK_ENEMY = CATEGORY_PLAYER | CATEGORY_ENEMY | CATEGORY_ABILITY | CATEGORY_WALLS;
const short MASK_COLLECTABLE = CATEGORY_PLAYER | CATEGORY_WALLS;
const short MASK_COLLECTSENSOR = CATEGORY_PLAYER | CATEGORY_WALLS;
const short MASK_SAFEAREA = CATEGORY_PLAYER;
const short MASK_ABILITY = CATEGORY_ENEMY | CATEGORY_WALLS;
const short MASK_WALLS = ~MASK_WALLS;

class Entity
{
	// Methods
public:
	virtual ~Entity();
	// Generates any static assets revelant to the specific entity. Parent
	// method should be called before child method, to ensure entities are
	// not re-initalised
	virtual bool Initialise(Renderer& renderer, FMOD::System& fmodSystem);
	virtual void Process(float deltaTime, Input& input) = 0;
	virtual void Draw(Renderer& renderer) = 0;
	virtual void SetPosition(Vector2 position);
#ifdef _DEBUG
	virtual void DebugDraw() = 0;
#endif // _DEBUG

	bool IsColliding(Vector2 position, float radius);
	bool IsLoaded() const;
	bool GetIsToBeDeleted();
	void SetIsToBeDeleted(bool state);
	void Unload();
	virtual Vector2 GetPosition();
	virtual Vector2 GetVelocity();
	ID GetId() const;
	std::string GetIdAsString();

	virtual void OnCollision(UserData* type);
	virtual void OnCollisionExit(UserData* type);

	float& GetCollisionRadius();

	float GetAngle();

	EntityType GetBodyEntityType();
protected:
	Entity();

private:
	Entity(const Entity& e);
	Entity& operator=(const Entity& e) {}

	// Data:
public:
protected:
	ID m_id;
	Vector2 m_position;
	Vector2 m_velocity;
	float m_fCollisionRadius;
	float m_fAngle;
	float m_fSpeed;
	bool m_bInitalised;
	bool m_bIsLoaded;
	bool m_bIsToBeDeleted;

	b2Body* m_pB2Body;
	b2Fixture* m_pB2Fixture;

	FMOD::System* m_pFMODsystem;
	UserData* m_pUserData;

private:
};

#endif // __ENTITY_H__