#pragma once

#include "ParticleForceGenerator.h"

class ParticleAnchoredSpring : public ParticleForceGenerator //Cette classe est un enfant de la classe ParticleForceGenerator
{
public :
	/*
	Le constructeur prends en argument deux float l'un pour la constante de raideur du ressort, l'autre la longueur au repos du ressort.
	Il prends également un Vecteur3D en argument qui corresponds au point d'ancrage du ressort
	*/
	ParticleAnchoredSpring(float sConst, float restL, Vector3D* anchor);
	~ParticleAnchoredSpring();

	//La fonction UpdateForce de ParticleForceGenerator est override
	virtual void UpdateForce(Particule* particle, float deltaTime);

private :
	float springConstant;
	float restLength;
	Vector3D *anchor;
};