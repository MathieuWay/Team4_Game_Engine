#pragma once
#include "ParticleForceGenerator.h"
#include "../Vector3D/Vector3D.h"

class ParticleGravity : public ParticleForceGenerator //Cette classe est un enfant de la classe ParticleForceGenerator
{
public:

	//L'instantiation de cette class ce fait avec une gravité qui est un vector3D
	ParticleGravity(const Vector3D& gravity);

	//La fonction UpdateForce de ParticleForceGenerator est override
	virtual void UpdateForce(Particule* particle , float deltaTime);


private:
	Vector3D m_gravity;
};