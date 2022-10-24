#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <engine/entity.h>
#include <vector2.h>
#include <vector>

class XInputController;
class Sprite;
class SpriteSheetAnimation;
class b2World;
class Player;

namespace NavMesh
{
	class PathFinder;
	class Point;
}

namespace ENEMY
{
	enum State
	{
		PATROLLING,
		CHASING,
		STUNNED,
	};
}

struct PathNode {
	Vector2 position;
	PathNode* nextNode;
};

class Enemy : public Entity
{
public:
	Enemy();
	virtual ~Enemy();

	// Inherited via Entity
	virtual bool Initialise(Renderer& renderer);
	void Setup(Player& player, PathNode* nodes, b2World& world, NavMesh::PathFinder& pathFinder);
	virtual void Process(float deltaTime, Input& input) override;

	void Stun(float duration);
	virtual void Draw(Renderer& renderer) override;
#ifdef _DEBUG
	virtual void DebugDraw() override;
#endif _DEBUG

	virtual void OnCollision(UserData* userData);

protected:
	bool IsPlayerInLOS();

	void PatrolProcess(float deltaTime);
	void ChaseProcess(float deltaTime);
	void StunProcess(float deltaTime);

private:

	Enemy(const Enemy& p);
	Enemy& operator=(const Enemy& p) {}

public:

protected:
	Sprite* m_pSprite;
	Sprite* m_pVisonConeSprite;
	Sprite* m_pDebugSprite;

	b2World* m_pB2World;

	ENEMY::State m_state;
	ENEMY::State m_previousState;
	Vector2 m_targetPosition;
	std::vector<NavMesh::Point> m_pathedPositionTargets;
	NavMesh::Point m_positionToMoveTo;
	PathNode* m_pPathNodes;
	PathNode* m_pCurrentNode;
	NavMesh::PathFinder* m_pPathFinder;

	Player* m_pPlayer;

	float m_fRunSpeedFactor;
	float m_fSearchAngle;
	float m_fWaitTime;
	float m_fSearchDistance;
	float m_fAngleToPlayer;

	float m_fUntilSpotTime;
	float m_fUntilSpottedDuration;

	float m_fPatrolReturnTime;
	float m_fPatrolReturnDuration;

	float m_fStunTime;
	float m_fStunDuration;

	bool m_bFreeze;
	bool m_bBlind;
	float m_fBlindTime;
	float m_fBlindDuration;
	bool m_bPlayerInLOS;

#ifdef _DEBUG
	Vector2 dm_Raycastp1;
	Vector2 dm_Raycastp2;
#endif //_DEBUG

private:

};

#endif // __ENEMY_H__

