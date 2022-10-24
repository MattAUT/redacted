// COMP710 GP Framework 
#ifndef __PARTICLEEMITTER_H__ 
#define __PARTICLEEMITTER_H__ 

// Library includes: 
#include <vector> 
#include "vector2.h"

// Forward declarations:  
class Renderer;
class Sprite;
class Particle;

// Class declaration:  
class ParticleEmitter
{
	// Member methods:  
public:
	ParticleEmitter();
	~ParticleEmitter();

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

	void Spawn();

	void DebugDraw();

	void SetPosition(Vector2 position);
	void SetSharedSprite(Sprite* sharedSprite);
	void SetEmissionRate(float newEmissionRate);
	void SetBatchSize(int newBatchSize);
	void SetAccelerationScalar(float newAccelerationScalar);
	void SetParticleLifeSpan(float newLifeSpan);
	void SetSharedSpriteScale(float newScale);
	void SetSpawnRange(float);
	void SetColour(float r, float g, float b);
	void ClearParticles();
	void SetActive(bool isActive);

protected:

private:
	ParticleEmitter(const ParticleEmitter& particleemitter);
	ParticleEmitter& operator=(const ParticleEmitter& particleemitter);

	// Member data:  
public:

protected:
	Sprite* m_pSharedSprite;
	std::vector<Particle*> m_particles;
	Renderer* m_pRenderer;

	bool m_bActive;
	bool iSshowDebug;
	float m_fTimeElapsed;

	float m_fSpawnRange;
	float m_fEmitRate;
	float m_fMaxLifespan;
	float m_fAccelerationScalar;
	float m_fColour[3];
	float m_fMinAngle;
	float m_fMaxAngle;
	Vector2 m_position;
	int m_iMaxScreenWidth;
	int m_iMaxScreenHeight;
	int m_iSpawnBatchSize;
private:

};

#endif // __PARTICLEEMITTER_H__ 