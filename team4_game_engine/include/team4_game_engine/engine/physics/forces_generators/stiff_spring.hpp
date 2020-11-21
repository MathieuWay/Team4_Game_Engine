#pragma once
#include "ParticleForceGenerator.h"

class ParticleStiffSpring : public ParticleForceGenerator 
{
private:

	Vector3D* m_anchor;
	float m_springConst;
	float m_damping;

public:

	ParticleStiffSpring(Vector3D* anchor, float springConst, float damping);

	~ParticleStiffSpring();

	virtual void UpdateForce(Particule* particle, float deltaTime);
};