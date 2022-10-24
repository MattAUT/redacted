#ifndef __ABILITYMANAGER_H__
#define __ABILITYMANAGER_H__

#include <vector>

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

class Ability;
class Renderer;
class Input;
class Player;

class AbilityManager
{
public:
	AbilityManager();
	~AbilityManager();

	bool Initialise(Renderer& renderer, Player* player);
	void Process(float deltaTime, Input& input);
	void Draw(Renderer& renderer);

	Ability* GetAbility(int i);
	std::vector<Ability*>* GetAllAbilities();

	void DebugDraw();

protected:
	bool m_bDebugSprites;

	Player* m_pPlayer;

	std::vector<Ability*> m_abilityVector;

	//Abilities
	Ability* m_pAbilitySmokeBomb;
	Ability* m_pAbilityFlashLight;
	Ability* m_pAbilityTranq;
};
#endif // !__ABILITYMANAGER_H__

