//#include "ParticleStiffSpring.h"
//#include <corecrt_math.h>
//
//ParticleStiffSpring::ParticleStiffSpring(Vector3D* anchor, float springConst, float damping)
//{
//	m_anchor = anchor;
//	m_springConst = springConst;
//	m_damping = damping;
//}
//
//ParticleStiffSpring::~ParticleStiffSpring() 
//{
//	delete m_anchor;
//}
//
//void ParticleStiffSpring::UpdateForce(Particule* particle, float deltaTime)
//{
//	//si la force est infini ou null on n'applique pas de force
//	if (particle->GetMass() <= 0) return;
//
//	//calcule de la position de la particule par rapport à l'encrage
//	Vector3D position = particle->GetPosition().subVector(*m_anchor);
//
//	//Calcule de la constante gamma
//	float gamma = 0.5f * sqrt((4 * (double)m_springConst) - ((double)m_damping * (double)m_damping));
//
//	//si la constante est null on n'applique pas de force
//	if (gamma == 0.0f) return;
//
//	//calcule de la constante "c"
//	Vector3D c = position.scalarMultiplication(m_damping / (2.0f * gamma)).sumVector(particle->GetVelocity().scalarMultiplication(1.0f / gamma));
//
//	//calcule de la prochaine position
//	Vector3D target = position.scalarMultiplication(cosf(gamma * deltaTime)).sumVector(c.scalarMultiplication(sinf(gamma * deltaTime)));
//	target = target.scalarMultiplication(expf(-0.5f * deltaTime * m_damping));
//
//	//calcule de l'acceleration pour atteindre la position et donc de la force
//	Vector3D accel = target.subVector(position).scalarMultiplication(1.0f / (deltaTime * deltaTime)).subVector(particle->GetVelocity().scalarMultiplication(deltaTime));
//
//	particle->AddForce(accel.scalarMultiplication(particle->GetMass()));
//}