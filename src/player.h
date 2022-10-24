#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <engine/entity.h>
#include <vector2.h>

class XInputController;
class Sprite;
class SpriteSheetAnimation;
class b2World;
class PlayerStorage;
class Collectable;
class AbilityManager;

class Player : public Entity
{
public:
	Player();
	virtual ~Player();

	// Inherited via Entity
	virtual bool Initialise(Renderer& renderer);
	virtual void Setup(Vector2 safeZoneCenter);
	virtual void Process(float deltaTime, Input& input) override;
	virtual void Draw(Renderer& renderer) override;
#ifdef _DEBUG
	virtual void DebugDraw() override;
#endif _DEBUG

	virtual void OnCollision(UserData* userData);


	bool GetIsPlayerFull();
	int GetStorageCurrentSize();
	PlayerStorage* GetPlayerStorage();

	void AddCollectable(Collectable* collectable);
	void OnCaught();
	int GetPlayerHP();
	bool IsPlayerDead();
	AbilityManager* GetAbilityManager();
private:

	Player(const Player& p);
	Player& operator=(const Player& p) {}

public:
	bool GetIsSneaking();

protected:
	void ProcessMovement();
	XInputController* m_pController;
	Vector2 m_movementInput;
	Vector2 m_rotationInput;
	Vector2 m_safeZoneCenter;

	Sprite* m_pSprite;
	Sprite* m_pDebugSprite;

	bool m_isSneaking;
	bool m_bDoOnCaught;

	float m_fSneakSpeed;

	//Deadzones
	float m_fLeftStickDeadzone;
	float m_fRighttickDeadzone;

	b2Fixture* m_pTestFixture;
	PlayerStorage* m_pPlayerStorage;
	AbilityManager* m_pPlayerAbilities;

	int m_iPlayerHP;

	FMOD::Sound* m_pDeathSound;
private:

};

#endif // !__PLAYER_H__

