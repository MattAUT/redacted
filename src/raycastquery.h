// https://gamedev.stackexchange.com/questions/179294/box2d-raycast-c-always-returning-false-trying-to-check-if-player-is-grounde
#ifndef  __RAYCASTCALLBACK_H__
#define  __RAYCASTCALLBACK_H__

#include <box2d/b2_world_callbacks.h>
#include <box2d/box2d.h>
#include <vector>

class b2World;
struct EntityUserData;

class RaycastQuery : public b2RayCastCallback
{
public:
	RaycastQuery();
	virtual float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction);

	bool bHit;
	b2Fixture* out_fixture;
	b2Vec2 out_point;
	b2Vec2 out_normal;
	float out_fraction;
};

#endif //  __RAYCASTCALLBACK_H__

