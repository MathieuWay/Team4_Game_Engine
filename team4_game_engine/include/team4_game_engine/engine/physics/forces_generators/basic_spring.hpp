#pragma once

#include "ParticleForceGenerator.h"

class ParticleBasicSpring : public ParticleForceGenerator //Cette classe est un enfant de la classe ParticleForceGenerator
{
public:
	/*
	Le constructeur prends en argument deux float l'un pour la constante de raideur du ressort, l'autre la longueur au repos du ressort. 
	Il prends également une particule en argument qui corresponds à l'autre particule liee au ressort
	*/
	ParticleBasicSpring(Particule* particule, float sConst, float restL);
	~ParticleBasicSpring();

	//La fonction UpdateForce de ParticleForceGenerator est override
	virtual void UpdateForce(Particule* particle, float deltaTime);

private:
	float springConstant;
	float restLength;
};