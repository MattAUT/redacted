#ifndef  __ABILITY_H__
#define __ABILITY_H__
#include "vector2.h"
#include <string>
#include <engine/xinputcontroller.h>
#include <engine/entity.h>

#include "utils.h"
class Player;
class Input;
class b2Body;
class Sprite;
class Renderer;
class b2Fixture;
class UserData;

class Ability : public Entity
{
public: 
	Ability();
	virtual ~Ability();
	virtual bool Initialise(Renderer& renderer, Player& player, SDL_GameControllerButton button) = 0;
	virtual void UseAbility(Vector2 pos) = 0;
	float GetCoolDownTimer();
	float GetCoolDown();
	virtual void DebugDraw() override;
protected:

private:
	Ability(const Ability& a);
	Ability& operator=(const Ability& a) {}

public:
	bool m_bIsDebuging;

protected:
	bool m_bIsActive;

	float m_fCoolDown;
	float m_fCoolDownTimer;
	
	float m_fDuration;
	float m_fDurationCounter;

	Vector2 m_position;

	Sprite* m_pSprite;
	Sprite* m_pDebugSprite;

	std::string m_sName;

	Player* m_pPlayer;

	SDL_GameControllerButton m_button;
};

#endif // ! __ABILITY_H__

