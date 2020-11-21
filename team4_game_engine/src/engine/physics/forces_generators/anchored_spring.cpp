//#include "ParticleAnchoredSpring.h"
//
//ParticleAnchoredSpring::ParticleAnchoredSpring(float sConst, float restL, Vector3D* anchor) {
//	this->anchor = anchor;
//	this->restLength = restL;
//	this->springConstant = sConst;
//}
//
//ParticleAnchoredSpring::~ParticleAnchoredSpring() 
//{
//	delete anchor;
//}
//
//void ParticleAnchoredSpring::UpdateForce(Particule* particle, float deltaTime) {
//	// On calcule le vecteur du ressort  
//	Vector3D force = particle->GetPosition().subVector(*anchor);
//
//	// On calcul sa norme pour calculer la force
//	float magnitude = force.magnitude();
//	magnitude = (restLength - magnitude) * springConstant;
//
//	// On applique la nouvelle force a la particule
//	force.normalize();
//	force = force.scalarMultiplication(magnitude);
//	particle->AddForce(force);
//}