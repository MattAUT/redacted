#include "raycastquery.h"
#include <engine/entity.h>
#include "logmanager.h"

RaycastQuery::RaycastQuery()
	: bHit(false)
	, out_fixture(nullptr)
{
}

float RaycastQuery::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{
	bHit = true;
	out_fixture = fixture;
	out_point = point;
	out_normal = normal;
	out_fraction = fraction;
	if (out_fixture == nullptr)
	{
		LogManager::GetInstance().Log("RCQ returning null fixture");
	}
	if (out_fixture != nullptr)
	{
		UserData* ud = reinterpret_cast<UserData*>(fixture->GetUserData().pointer);
		if (ud != NULL)
		{
			if (ud->m_type == ET_COLLECTABLE || ud->m_type == ET_COLLECTSENSOR)
			{
				return 1;
			}
			else
			{
				LogManager::GetInstance().Log(EntityTypeToString(ud->m_type));
				return fraction;
			}
		}
	}

	return fraction;
}