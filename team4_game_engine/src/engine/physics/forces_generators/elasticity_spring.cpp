//#include "ParticleElasticity.h"
//
//ParticleElasticity::ParticleElasticity(Particule* other, float sConst, float rLength) : springConst(sConst), restLength(rLength)
//{
//	secondPart = other;
//}
//
//ParticleElasticity::~ParticleElasticity() 
//{
//	delete secondPart;
//}
//
//
//void ParticleElasticity::UpdateForce(Particule* particule, float deltaTime)
//{
//	Vector3D force = secondPart->GetPosition().subVector(particule->GetPosition());
//
//	float magnitude = force.magnitude();
//
//	// Test si l'élastique est compressé. Calcule la magnitude si ce n'est pas le cas.
//	if (magnitude <= restLength) return;
//
//	magnitude = springConst * (restLength - magnitude);
//
//	// Applique la force
//	force.normalize();
//	force = force.scalarMultiplication(-magnitude);
//	particule->AddForce(force);
//}
//
//// Getter and Setter
//float ParticleElasticity::GetSpringConst()
//{
//	return springConst;
//}
//
//void ParticleElasticity::SetSpringConst(float _springConst)
//{
//	springConst = _springConst;
//}
//
//float ParticleElasticity::GetRestLength()
//{
//	return restLength;
//}
//
//void ParticleElasticity::SetRestLength(float _restLength)
//{
//	restLength = _restLength;
//}