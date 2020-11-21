#pragma once

#include "ParticleForceGenerator.h"
#include "../Vector3D/Vector3D.h"

class ParticleDrag : public ParticleForceGenerator //Cette classe est un enfant de la classe ParticleForceGenerator
{
public:

	//L'instantiation de cette class ce fait avec deux float
	ParticleDrag(float k1, float k2);

	//La fonction UpdateForce de ParticleForceGenerator est override
	virtual void UpdateForce(Particule* particle, float deltaTime);


private:
	float m_k1;
	float m_k2;
};