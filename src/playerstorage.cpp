#include "playerstorage.h"
#include "logmanager.h"

PlayerStorage::PlayerStorage()
	:m_iCurrentSize(0)
	, m_iMaxSize(1)
{
}

PlayerStorage::~PlayerStorage()
{
	m_pPouch.clear();

}

int PlayerStorage::GetCurrentSize()
{
	return m_pPouch.size();
}

bool PlayerStorage::GetIsFull()
{
	if (m_iCurrentSize >= m_iMaxSize)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PlayerStorage::AddItem(Collectable * collectable)
{
	if (m_iCurrentSize < m_iMaxSize)
	{
		m_pPouch.push_back(collectable);
		LogManager::GetInstance().Log("Added collectable to bag");
	}
	else
	{
		LogManager::GetInstance().Log("Bag Full");
	}
}

std::vector<Collectable*> PlayerStorage::GetPouch()
{
	return m_pPouch;
}

void PlayerStorage::ClearPouch()
{
	//for (Collectable* item : m_pPouch)
	//{
	//	//delete item;
	//}
	m_pPouch.clear();
}

