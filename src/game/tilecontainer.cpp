// This include: 					  
#include "tilecontainer.h"

#include "tile.h"

#include "vector2.h"
#include <engine/renderer.h>
#include <engine/sprite.h>
#include <engine/scenemanager.h>
#include <engine/camera.h>

TileContainer::TileContainer()
	: m_pCamera(SceneManager::GetInstance().GetCamera())
{

}

TileContainer::~TileContainer()
{
	for (Tile* tile : m_vTileArray)
	{
		delete tile;
	}
	m_vTileArray.clear();
}

void
TileContainer::AddTile(Tile* tile)
{
	m_vTileArray.push_back(tile);
}

void
TileContainer::Draw(Renderer& renderer)
{
	for (unsigned int num = 0; num < m_vTileArray.size(); num++)
	{
		if ((m_vTileArray[num]->GetPosition().x > (-m_pCamera->GetPosition().x - 25)) && (m_vTileArray[num]->GetPosition().x < (-m_pCamera->GetPosition().x + 25)))
		{
			if ((m_vTileArray[num]->GetPosition().y > (m_pCamera->GetPosition().y - 20)) && (m_vTileArray[num]->GetPosition().y < (m_pCamera->GetPosition().y + 20)))
			{
				m_vTileArray[num]->Draw(renderer);
			}
		}
	}
}