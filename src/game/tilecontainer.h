#ifndef __TILECONTAINER_H__				 
#define __TILECONTAINER_H__ 				 

// Class declaration: 			
class Renderer;
class Tile;
class Camera;

#include <vector>

class TileContainer
{
	// Member methods: 								 
public:
	TileContainer();
	~TileContainer();
	void AddTile(Tile* tile);
	void Draw(Renderer& renderer);

protected:

private:
	TileContainer(const TileContainer& t);
	TileContainer& operator=(const TileContainer& t);

	// Member data: 								 
public:

protected:
	std::vector<Tile*> m_vTileArray;

private:
	Camera* m_pCamera;
};

#endif // __TILECONTAINER_H__			 
