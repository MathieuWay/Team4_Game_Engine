#pragma once
#include "ParticleForceGenerator.h"

class ParticleElasticity : public ParticleForceGenerator
{
protected:
	float springConst;
	float restLength;

public:
	ParticleElasticity(Particule* other, float springConst, float restLength);
	~ParticleElasticity();

	virtual void UpdateForce(Particule* particule, float deltaTime);

	float GetSpringConst();
	void SetSpringConst(float _springConst);

	float GetRestLength();
	void SetRestLength(float _restLength);
};