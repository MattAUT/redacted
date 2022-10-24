#ifndef __COLLISIONCREATION_H__				 
#define __COLLISIONCREATION_H__ 				 

#include <engine/scene.h>
#include <vector2.h>
#include <path_finder.h>

// Class declaration: 			
class Renderer;
class Sprite;
class Vector2;
class b2World;
class b2Body;
class b2Fixture;

class CollisionCreation
{
	// Member methods: 								 
public:
	CollisionCreation();
	~ CollisionCreation();
	void Initialise(Vector2 spawnLocation, int boxCount, bool directionLeft, Renderer& renderer);

protected:

private:
	CollisionCreation(const CollisionCreation& t);
	CollisionCreation& operator=(const CollisionCreation& t);

	// Member data: 								 
public:

protected:
	b2World* m_pWorld;
	b2Body* m_testGroundBody;
	b2Fixture* m_pTestFixture;

private:

};

#endif // __COLLISIONCREATION_H__			 
