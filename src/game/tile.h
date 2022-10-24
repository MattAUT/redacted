#ifndef __TILE_H__				 
#define __TILE_H__ 				 

// Class declaration: 			
class Renderer;
class Sprite;
class Vector2;

class Tile
{
	// Member methods: 								 
public:
	Tile();
	~Tile();
	bool Initialise(Sprite* tileSprite, float x, float y);
	void Draw(Renderer& renderer);
	Vector2 GetPosition();

protected:

private:
	Tile(const Tile& t);
	Tile& operator=(const Tile& t);

	// Member data: 								 
public:

protected:
	Sprite* m_pTileSprite;
	Vector2* m_pPosition;

private:

};

#endif // __TILE_H__			 
