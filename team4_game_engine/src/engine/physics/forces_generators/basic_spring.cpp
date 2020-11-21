//#include "ParticleBasicSpring.h"
//#include "../../Scene.h"
//#include <corecrt_math.h>
//
//ParticleBasicSpring::ParticleBasicSpring(Particule* other, float sConst, float restL) : restLength(restL), springConstant(sConst){
//	secondPart = other;
//}
//
//ParticleBasicSpring::~ParticleBasicSpring() 
//{
//	delete secondPart;
//}
//
//void ParticleBasicSpring::UpdateForce(Particule* particle, float deltaTime) {
//	Vector3D force = particle->GetPosition().subVector(secondPart->GetPosition());
//
//	// On calcul sa norme pour calculer la force
//	float magnitude = force.magnitude();
//	magnitude = (restLength - magnitude) * springConstant;
//
//	// On applique la nouvelle force a la particule
//	force.normalize();
//	force = force.scalarMultiplication(magnitude);
//
//	if (Scene::Instance()->useMassWithSpring) force = force.scalarMultiplication(particle->GetInverseMass());
//	particle->AddForce(force);
//}