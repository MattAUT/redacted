#ifndef __HUD_H__
#define __HUD_H__

#include <vector>

class Renderer;
class Input;
class Sprite;
class Player;
class SafeArea;

class HUD
{
public:
	HUD();
	~HUD();

	// Inherited via Entity
	bool Initialise(Renderer& renderer, Player* player, SafeArea* safeArea);
	void Process(float deltaTime, Input& input);
	void Draw(Renderer& renderer);
	void DrawPlayerHearts(Renderer& renderer);
	void DrawRetrievedItems(Renderer& renderer);
	void DrawPlayerHeldItems(Renderer& renderer);
#ifdef _DEBUG
	void DebugDraw();
#endif _DEBUG
private:
	HUD(const HUD& p);
	HUD& operator=(const HUD& p) {}

public:

protected:
	Player* m_pPlayer;
	SafeArea* m_pSafeArea;

	int m_iPlayerHP;

	Sprite* m_pHPTextSprite;
	Sprite* m_pHPSprite;

	Sprite* m_pSafeAreaItemCountText;
	Sprite* m_pItemSprite;

	Sprite* m_pObjectiveText;
	Sprite* m_pObjectiveCount;

	Sprite* m_pPlayerInventory;

	Sprite* m_pWinText;
	float m_fWinTextAlpha;

	Sprite* m_pCoolDownBar;
	std::vector<Sprite*> m_vCoolDowns;
	std::vector<Sprite*> m_vCoolDownIcons;

private:

};

#endif // !__HUD_H__

