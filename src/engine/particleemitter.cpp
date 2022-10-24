// COMP710 GP Framework 

// This include:  
#include "particleemitter.h"

// Local includes:  
#include "sprite.h"
#include "particle.h"
#include "renderer.h"
#include "imgui/imgui.h"
#include "utils.h"
#include "vector2.h"

ParticleEmitter::ParticleEmitter()
	: m_pSharedSprite(0)
	, m_bActive(false)
	, m_fSpawnRange(1)
	, iSshowDebug(false)
{

}

ParticleEmitter::~ParticleEmitter()
{
	delete m_pSharedSprite;
	m_pSharedSprite = 0;

	for (Particle* particle : m_particles)
	{
		delete particle;
	}
	m_particles.clear();
}

bool
ParticleEmitter::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;

	m_fTimeElapsed = 0;
	m_iSpawnBatchSize = 1;
	m_fEmitRate = 1;
	m_fMaxLifespan = 3;
	m_fAccelerationScalar = 2;
	m_fColour[0] = 1;
	m_fColour[1] = 1;
	m_fColour[2] = 1;
	m_fMinAngle = 0;
	m_fMaxAngle = 360;

	m_iMaxScreenWidth = renderer.GetWidth();
	m_iMaxScreenHeight = renderer.GetHeight();

	m_position.x = -5;
	m_position.y = -5;

	return true;
}

void
ParticleEmitter::SetSharedSprite(Sprite* sharedSprite)
{
	m_pSharedSprite = sharedSprite;
}

void
ParticleEmitter::SetEmissionRate(float newEmissionRate)
{
	m_fEmitRate = newEmissionRate;
}

void
ParticleEmitter::Process(float deltaTime)
{
	// Emit new particles
	if (m_fTimeElapsed > m_fEmitRate && m_bActive)
	{
		for (int j = 0; j < m_iSpawnBatchSize; j++)
		{
			Spawn();
		}
		m_fTimeElapsed = 0;
	}
	for (unsigned int i = 0; i < m_particles.size(); i++)
	{
		// Process each particle
		m_particles.at(i)->Process(deltaTime);

		// Handle dead particles
		if (!m_particles.at(i)->m_bAlive)
		{
			delete m_particles.at(i);
			m_particles.erase(m_particles.begin() + i);
		}
	}

	m_fTimeElapsed += deltaTime;
}

void
ParticleEmitter::Draw(Renderer& renderer)
{
	//for (Particle* particle : m_particles)
	//{
	//	particle->Draw(renderer);
	//}


	for (int i = m_particles.size() - 1; i >= 0; i--)
	{
		m_particles[i]->Draw(renderer);
	}
}

void
ParticleEmitter::SetPosition(Vector2 position)
{
	m_position = position;
}

void
ParticleEmitter::SetBatchSize(int newBatchSize)
{
	m_iSpawnBatchSize = newBatchSize;
}

void ParticleEmitter::SetAccelerationScalar(float newAccelerationScalar)
{
	m_fAccelerationScalar = newAccelerationScalar;
}

void ParticleEmitter::SetParticleLifeSpan(float newLifeSpan)
{
	m_fMaxLifespan = newLifeSpan;
}

void
ParticleEmitter::SetSharedSpriteScale(float newScale)
{
	m_pSharedSprite->SetScale(newScale);
}

void ParticleEmitter::SetSpawnRange(float range)
{
	m_fSpawnRange = range;
}

void ParticleEmitter::SetColour(float r, float g, float b)
{
	m_fColour[0] = r;
	m_fColour[1] = g;
	m_fColour[2] = b;
}

void ParticleEmitter::ClearParticles()
{
	for (Particle* particle : m_particles)
	{
		delete particle;
	}
	m_particles.clear();
}

void ParticleEmitter::SetActive(bool isActive)
{
	m_bActive = isActive;
}

void
ParticleEmitter::Spawn()
{
	Particle* particle = new Particle();
	particle->Initialise(*m_pSharedSprite);

	particle->m_bAlive = true;
	particle->m_fMaxLifespan = m_fMaxLifespan;

	float randomX = (GetRandom_f(m_position.x - m_fSpawnRange, m_position.x + m_fSpawnRange));
	float randomY = (GetRandom_f(m_position.y - m_fSpawnRange, m_position.y + m_fSpawnRange));

	particle->m_postion = Vector2(randomX, randomY);

	float randomAngle = DegToRad(static_cast<float>(GetRandom(static_cast<int>(m_fMinAngle), static_cast<int>(m_fMaxAngle))));
	float RandomRadius = m_fAccelerationScalar * GetRandom(1, 50);
	float accelerationX = RandomRadius * cosf(randomAngle);
	float accelerationY = RandomRadius * sinf(randomAngle);

	particle->m_acceleration.Set(accelerationX, accelerationY);

	for (int i = 0; i < 3; i++)
	{
		particle->m_fColour[i] = m_fColour[i];
	}

	m_particles.push_back(particle);
}

void
ParticleEmitter::DebugDraw()
{
		ImGui::Checkbox("Show Emmiter Debug", &iSshowDebug);
	if (iSshowDebug)
	{
		// General Particle Settings
		ImGui::SliderInt("Spawn Batch Size", &m_iSpawnBatchSize, 0, 5000);
		ImGui::SliderFloat("Emission Rate", &m_fEmitRate, 0.01f, 1);
		ImGui::SliderFloat("Max Lifespan", &m_fMaxLifespan, 0, 10);
		ImGui::SliderFloat("Acceleration Scalar", &m_fAccelerationScalar, 0.1, 5);

		// Colors
		ImGui::SliderFloat("Color Red", &m_fColour[0], 0, 1);
		ImGui::SliderFloat("Color Green", &m_fColour[1], 0, 1);
		ImGui::SliderFloat("Color Blue", &m_fColour[2], 0, 1);

		// Spawn Angle
		ImGui::SliderFloat("Min Angle", &m_fMinAngle, 0, 360);
		ImGui::SliderFloat("Max Angle", &m_fMaxAngle, 0, 360);

		// Position
		//ImGui::SliderFloat("Position X", &m_position.x, -100, 100);
		//ImGui::SliderFloat("Position Y", &m_position.y, -100, 100);
		ImGui::SliderFloat("Position Y", &m_fSpawnRange, 0, 30);

		// Particle Count
		ImGui::Text("Particles (%d)", m_particles.size());

		//ImGui::ShowDemoWindow();
	}
}