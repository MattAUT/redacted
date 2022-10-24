// This include: 					  
#include "tile.h"    

#include "vector2.h"
#include <engine/renderer.h>
#include <engine/sprite.h>

Tile::Tile()
	: m_pTileSprite(0)
	, m_pPosition(0)
{

}

Tile::~Tile()
{
	delete m_pPosition;
	m_pPosition = 0;
}

bool
Tile::Initialise(Sprite* tileSprite, float x, float y)
{
	m_pTileSprite = tileSprite;
	m_pPosition = new Vector2(x, y);

	return true;
}

void
Tile::Draw(Renderer& renderer)
{
	m_pTileSprite->Draw(renderer, *m_pPosition, 0.0f);
}

Vector2
Tile::GetPosition()
{
	return *m_pPosition;
}
