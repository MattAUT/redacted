#ifndef __ABILITYTRANQDART_H__
#define __ABILITYTRANQDART_H__

#include "ability.h"
#include <engine/entity.h>

enum EntityType;

class AbilityTranqDart : public Ability
{
public:
	AbilityTranqDart();
	~AbilityTranqDart();

	// Inherited via Ability

	virtual void UseAbility(Vector2 pos) override;

	virtual bool Initialise(Renderer & renderer, Player & player, SDL_GameControllerButton button);

	virtual void Draw(Renderer & renderer) override;

	virtual void Process(float deltaTime, Input & input) override;

	virtual void DebugDraw() override;

	void OnCollision(UserData* userData);
private:
	AbilityTranqDart(const AbilityTranqDart& e) {}
	AbilityTranqDart& operator=(const AbilityTranqDart& e) {}
	
	Vector2 m_direction;
	float m_fSpeed;

	FMOD::Sound* m_pDartSound;
	FMOD::Sound* m_pJumpSound;

	float m_fDartAngle;
};

#endif // !__ABILITYTRANQDART_H__

