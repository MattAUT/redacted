#ifndef __CURRENTLEVELLOADED_H__
#define __CURRENTLEVELLOADED_H__

#include <engine/entity.h>
#include <vector2.h>
#include <vector>

class XInputController;
class Sprite;
class SpriteSheetAnimation;
class PlayerStorage;
class Collectable;
class AbilityManager;
class UserData;
class b2Fixture;

class CurrentLevelLoaded : public Entity
{
public:
	CurrentLevelLoaded();
	~CurrentLevelLoaded();

	// Inherited via Entity
	virtual bool Initialise(Renderer& renderer, Vector2 position);
	virtual void Process(float deltaTime, Input & input) override;
	virtual void Draw(Renderer & renderer) override;
	
#ifdef _DEBUG
	void DebugDraw();
#endif _DEBUG

	void OnCollision(UserData* type);
	void OnCollisionExit(UserData* type);
	bool GetPlayerInArea();

private:

	CurrentLevelLoaded(const CurrentLevelLoaded& p);
	CurrentLevelLoaded& operator=(const CurrentLevelLoaded& p) {}

public:

protected:

	// Container
	std::vector<Collectable*> m_vStash;

	// Box2D
	b2Fixture* m_pSafeAreaDetector;

	bool isPlayerInArea;

	b2Body* m_pB2Body;
	Vector2 m_position;
	UserData* m_pUserData;

private:


	

};

#endif // __CURRENTLEVELLOADED_H__

