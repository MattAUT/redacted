// COMP710 Les Mcilroy

#include "collisionlistener.h"
#include "box2d/box2d.h"
#include <engine/entity.h>
#include <logmanager.h>
#include <cstdint>
#include "collectable.h"

#include "abilitytranqdart.h"

CollisionListener::CollisionListener()
{

}

CollisionListener::~CollisionListener()
{
}


void CollisionListener::BeginContact(b2Contact * contact)
{
	b2FixtureUserData fixtureDataA = contact->GetFixtureA()->GetUserData();
	b2FixtureUserData fixtureDataB = contact->GetFixtureB()->GetUserData();

	UserData* userDataA = reinterpret_cast<UserData*>(fixtureDataA.pointer);
	UserData* userDataB = reinterpret_cast<UserData*>(fixtureDataB.pointer);

	if (userDataA != NULL && userDataB != NULL)
	{
		if (userDataB->m_type == ET_PLAYER || userDataA->m_type == ET_PLAYER)
		{
			if (userDataA->m_type == ET_ENEMY)
			{
				userDataB->m_pEntityPointer->OnCollision(userDataA);
			}
			if (userDataB->m_type == ET_ENEMY)
			{
				userDataA->m_pEntityPointer->OnCollision(userDataB);
			}

			if (userDataA->m_type == ET_COLLECTABLE)
			{
				userDataA->m_pEntityPointer->OnCollision(userDataB);
			}
			if (userDataB->m_type == ET_COLLECTABLE)
			{
				userDataB->m_pEntityPointer->OnCollision(userDataA);
			}

			if (userDataA->m_type == ET_SAFEAREA)
			{
				userDataA->m_pEntityPointer->OnCollision(userDataB);
			}
			if (userDataB->m_type == ET_SAFEAREA)
			{
				userDataB->m_pEntityPointer->OnCollision(userDataA);
			}

			if (userDataA->m_type == ET_COLLECTSENSOR)
			{
				reinterpret_cast<Collectable*>(userDataA->m_pEntityPointer)->OnCollisionSensor(userDataB);
			}
			if (userDataB->m_type == ET_COLLECTSENSOR)
			{
				reinterpret_cast<Collectable*>(userDataB->m_pEntityPointer)->OnCollisionSensor(userDataA);
			}
		}

		if (userDataB->m_type == ET_ENEMY || userDataA->m_type == ET_ENEMY)
		{
			if (userDataA->m_type == ET_TRANQDART)
			{
				(userDataA->m_pEntityPointer)->OnCollision(userDataB);
				(userDataB->m_pEntityPointer)->OnCollision(userDataA);
			}
			if (userDataB->m_type == ET_TRANQDART)
			{
				(userDataB)->m_pEntityPointer->OnCollision(userDataA);
				(userDataA->m_pEntityPointer)->OnCollision(userDataB);
			}

			if (userDataA->m_type == ET_FLASHLIGHT)
			{
				(userDataA->m_pEntityPointer)->OnCollision(userDataB);
			}
			if (userDataB->m_type == ET_FLASHLIGHT)
			{
				(userDataB->m_pEntityPointer)->OnCollision(userDataA);
			}

			if (userDataA->m_type == ET_SMOKEBOMB)
			{
				(userDataB->m_pEntityPointer)->OnCollision(userDataA);
			}
			if (userDataB->m_type == ET_SMOKEBOMB)
			{
				(userDataA->m_pEntityPointer)->OnCollision(userDataB);
			}
		}

		if (userDataB->m_type == ET_TRANQDART || userDataA->m_type == ET_TRANQDART)
		{
			if (userDataA->m_type == ET_WALL)
			{
				userDataB->m_pEntityPointer->OnCollision(userDataB);
			}
			if (userDataB->m_type == ET_WALL)
			{
				userDataA->m_pEntityPointer->OnCollision(userDataA);
			}
		}
	}
}

void CollisionListener::EndContact(b2Contact * contact)
{
	b2FixtureUserData fixtureDataA = contact->GetFixtureA()->GetUserData();
	b2FixtureUserData fixtureDataB = contact->GetFixtureB()->GetUserData();

	UserData* userDataA = reinterpret_cast<UserData*>(fixtureDataA.pointer);
	UserData* userDataB = reinterpret_cast<UserData*>(fixtureDataB.pointer);

	if (userDataA != NULL && userDataB != NULL)
	{
		if (userDataB->m_type == ET_PLAYER || userDataA->m_type == ET_PLAYER)
		{
			if (userDataA->m_type == ET_COLLECTSENSOR)
			{
				userDataA->m_pEntityPointer->OnCollisionExit(userDataB);
			}
			if (userDataB->m_type == ET_COLLECTSENSOR)
			{
				userDataB->m_pEntityPointer->OnCollisionExit(userDataA);
			}

			if (userDataA->m_type == ET_SAFEAREA)
			{
				userDataA->m_pEntityPointer->OnCollisionExit(userDataB);
			}
			if (userDataB->m_type == ET_SAFEAREA)
			{
				userDataB->m_pEntityPointer->OnCollisionExit(userDataA);
			}

			if (userDataA->m_type == ET_COLLECTSENSOR)
			{
				reinterpret_cast<Collectable*>(userDataA->m_pEntityPointer)->OnCollisionSensorExit(userDataB);
			}
			if (userDataB->m_type == ET_COLLECTSENSOR)
			{
				reinterpret_cast<Collectable*>(userDataB->m_pEntityPointer)->OnCollisionSensorExit(userDataA);
			}
		}

		if (userDataB->m_type == ET_ENEMY || userDataA->m_type == ET_ENEMY)
		{
			if (userDataB->m_type == ET_FLASHLIGHT)
			{
				userDataB->m_pEntityPointer->OnCollisionExit(userDataA);
			}
			if (userDataA->m_type == ET_FLASHLIGHT)
			{
				userDataA->m_pEntityPointer->OnCollisionExit(userDataB);
			}
		}
	}
}
