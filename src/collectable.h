#ifndef __COLLECTABLE
#define __COLLECTABLE

#include <engine/entity.h>

class SmokeEmitter;
class Sprite;
class b2Fixture;
class Player;

class Collectable : public Entity
{
public:
	Collectable();
	virtual ~Collectable();

	// Inherited via Entity
	virtual bool Initialise(Renderer& renderer, Player& player, Vector2 position);
	virtual void Process(float deltaTime, Input& input) override;
	virtual void Draw(Renderer& renderer) override;
#ifdef _DEBUG
	virtual void DebugDraw() override;
#endif _DEBUG

	virtual void OnCollision(UserData* userData);
	virtual void OnCollisionExit(UserData* userData);

	void OnCollisionSensor(UserData* userData);
	void OnCollisionSensorExit(UserData* userData);

	bool GetIsCollected();
	bool GetIsHeld();
	bool GetIsPlayerInRange();

	void SetCollected();

	b2Body* GetBody();

private:
	Collectable(const Collectable& p);
	Collectable& operator=(const Collectable& p) {}

public:

protected:
	bool m_bIsCollected;
	bool m_bIsPlayerInRange;
	bool m_bIsPlayerFull;
	bool m_bIsHeld;

	float m_fSpeed;

	Sprite* m_pSprite;

	UserData* sensorUserData;
	b2Fixture* m_pPickUpSensor;

	Player* m_pPlayer;

	Sprite* m_pCollectionArea;

	FMOD::Sound* m_pCoinSound;
};

#endif // !__COLLECTABLE

