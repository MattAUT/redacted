#ifndef __SAFEAREA_H__
#define __SAFEAREA_H__

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

class SafeArea : public Entity
{
public:
	SafeArea();
	virtual ~SafeArea();

	// Inherited via Entity
	virtual bool Initialise(Renderer& renderer, Vector2 position, PlayerStorage* playerStorage, const char* safeAreaFilename, int numOfItemComplete);
	virtual void Process(float deltaTime, Input& input) override;
	virtual void Draw(Renderer& renderer) override;
#ifdef _DEBUG
	virtual void DebugDraw() override;
#endif _DEBUG

	virtual void OnCollision(UserData* userData);
	virtual void OnCollisionExit(UserData* userData);

	void StoreCollectables(PlayerStorage* storage);
	int GetStashCount();
	int GetObjectiveCount();
	bool isLevelComplete();

private:

	SafeArea(const SafeArea& p);
	SafeArea& operator=(const SafeArea& p) {}

public:

protected:
	Sprite* m_pSprite;
	Sprite* m_pDebugSprite;

	// Container
	std::vector<Collectable*> m_vStash;

	// Box2D
	b2Fixture* m_pSafeAreaDetector;

	PlayerStorage* m_pPlayerStorage;
	bool isPlayerInSafeArea;

	int m_iNumOfItemComplete;

private:

};

#endif // __SAFEAREA_H__

