// This include: 					  
#include "collisionCreation.h"    

#include "vector2.h"
#include <engine/renderer.h>
#include <engine/sprite.h>
#include <engine/scenemanager.h>

CollisionCreation::CollisionCreation()
{

}

CollisionCreation::~CollisionCreation()
{

}

void
CollisionCreation::Initialise(Vector2 spawnLocation, int boxCount, bool directionLeft, Renderer& renderer)
{
	m_pWorld = SceneManager::GetInstance().m_pWorld;
	//Go through the text files contents saving each character to use as a tile determiner for the map tiles
	b2BodyDef bodyDef2;
	b2PolygonShape groundShape;
	b2FixtureDef fixtureDef2;
	NavMesh::Polygon navMeshPolygon;

	//Fixture def
	if (directionLeft == true)
	{
		bodyDef2.position.Set((spawnLocation.x - ((boxCount) * 0.5)) - 0.5, (-spawnLocation.y));
		m_testGroundBody = m_pWorld->CreateBody(&bodyDef2);
		groundShape.SetAsBox((0.5 + (0.5 * (boxCount - 1))), 0.5);
	}
	else if (directionLeft == false)
	{
		bodyDef2.position.Set(spawnLocation.x, -(spawnLocation.y - ((boxCount) * 0.5)) + 0.5);
		m_testGroundBody = m_pWorld->CreateBody(&bodyDef2);
		groundShape.SetAsBox(0.5, (0.5 + (0.5 * (boxCount - 1))));
	}

	fixtureDef2.shape = &groundShape;
	fixtureDef2.friction = 0.3f;
	m_pTestFixture = m_testGroundBody->CreateFixture(&fixtureDef2);
}
