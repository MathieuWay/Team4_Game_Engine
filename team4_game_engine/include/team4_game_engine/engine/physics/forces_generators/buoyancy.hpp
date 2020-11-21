#pragma once
#include "ParticleForceGenerator.h"

class ParticleBuoyancy : public ParticleForceGenerator
{
private:

	float m_maxDepth;
	float m_volume;
	float m_waterHeight;
	float m_liquideDensity;

public:

	ParticleBuoyancy(float maxDepth, float volume, float waterHeight, float liquideDensity = 1000.0f);

	virtual void UpdateForce(Particule* particle, float deltaTime);
};