#ifndef __PLAYERSTORAGE_H__
#define __PLAYERSTORAGE_H__

#include "player.h"
#include <vector>

class Collectable;

class PlayerStorage
{
public:
	PlayerStorage();
	~PlayerStorage();

	int GetCurrentSize();
	bool GetIsFull();
	void AddItem(Collectable* collectable);
	
	std::vector<Collectable*> GetPouch();
	void ClearPouch();

private:
	PlayerStorage(const PlayerStorage& p);
	PlayerStorage& operator=(const PlayerStorage& p) {}

	std::vector<Collectable*> m_pPouch;
	int m_iMaxSize;
	int m_iCurrentSize;
};
#endif // !__PLAYERSTORAGE_H__


