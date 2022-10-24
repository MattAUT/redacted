#include "raycastcallback.h"
#include <logmanager.h>
#include <engine/entity.h>

RaycastCallback::RaycastCallback()
{
	bHit = false;
}

float RaycastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{
	bHit = true;
	out_fixture = fixture;
	out_point = point;
	out_normal = normal;
	out_fraction = fraction;

	return 0; 
}