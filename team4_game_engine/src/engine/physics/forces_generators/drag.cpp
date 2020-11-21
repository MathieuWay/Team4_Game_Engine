//#include "ParticleDrag.h"
//
//ParticleDrag::ParticleDrag(float k1, float k2) 
//{
//	m_k1 = k1;
//	m_k2 = k2;
//}
//
//void ParticleDrag::UpdateForce(Particule* particle, float deltaTime) 
//{
//	Vector3D force = particle->GetVelocity();
//	float dragCoeff = force.magnitude();
//	force.normalize();
//
//	dragCoeff =  m_k1 * dragCoeff + m_k2 * dragCoeff * dragCoeff;
//
//	force = force.scalarMultiplication(-dragCoeff);
//
//	particle->AddForce(force);
//}