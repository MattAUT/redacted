#ifndef __ABILITYSMOKEBOBMB_H__
#define __ABILITYSMOKEBOBMB_H__

#include "ability.h"

class ParticleEmitter;

class AbilitySmokeBomb : public Ability
{
public:
	AbilitySmokeBomb();
	virtual ~AbilitySmokeBomb();

	// Inherited via Ability
	virtual void UseAbility(Vector2 pos) override;

	virtual bool Initialise(Renderer& renderer, Player & player, SDL_GameControllerButton button) override;

	virtual void Draw(Renderer & renderer) override;

	virtual void Process(float deltaTime, Input& input) override;

	virtual void DebugDraw() override;

protected:

private:
	AbilitySmokeBomb(const AbilitySmokeBomb& e);
	AbilitySmokeBomb& operator=(const AbilitySmokeBomb& e) {}

	FMOD::Sound* m_pSmokeSound;
	ParticleEmitter* m_pParticleEmitter;
	Sprite* m_pParticleSprite;
};

#endif // !__SMOKEBOBMB_H__

