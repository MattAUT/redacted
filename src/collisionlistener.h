// COMP710 Les Mcilroy
#ifndef  __COLLISIONLISTENER_H__
#define  __COLLISIONLISTENER_H__

#include "box2d/b2_world_callbacks.h"
#include <vector>

class b2World;
struct EntityUserData;

class CollisionListener : public b2ContactListener
{
public:
	CollisionListener();
	virtual ~CollisionListener();

protected:
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);

private:
	CollisionListener(const CollisionListener& attackListener);
	CollisionListener& operator=(const CollisionListener& attackListener);


public:
};


#endif //  __COLLISIONLISTENER_H__
