#ifndef __ABILITYFLASHLIGHT_H__
#define __ABILITYFLASHLIGHT_H__

#include "ability.h"
#include <vector>

class Enemy;

class AbilityFlashLight : public Ability
{
public:
	AbilityFlashLight();
	~AbilityFlashLight();

	// Inherited via Ability
	virtual void UseAbility(Vector2 pos) override;

	virtual bool Initialise(Renderer& renderer, Player & player, SDL_GameControllerButton button) override;

	virtual void Draw(Renderer & renderer) override;

	virtual void Process(float deltaTime, Input& input) override;

	virtual void DebugDraw() override;

	virtual void OnCollision(UserData* userData) override;
	virtual void OnCollisionExit(UserData* userData) override;
private:
	AbilityFlashLight(const AbilityFlashLight& e);
	AbilityFlashLight& operator=(const AbilityFlashLight& e) {}
	bool CheckIfEnemiesAreInLOS(Enemy* enemy);

protected:
	bool m_bIsHeld;
	bool m_bSeesEnemy;

	bool m_bCheck;

	float m_fStunDuration;
	int m_fEnemyCount;

	float m_fAngle;
	float m_distance;
	Sprite* m_pFlash;
	Vector2 m_direction;
	Vector2 m_box2dPos;
	b2World *m_pB2World;

	std::vector<Enemy*> m_mEnemies;

	FMOD::Sound* m_pFlashSound;
};

#endif // !__ABILITYFLASHLIGHT_H__

