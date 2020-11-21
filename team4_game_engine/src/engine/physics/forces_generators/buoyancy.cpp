//#include "ParticleBuoyancy.h"
//
//ParticleBuoyancy::ParticleBuoyancy(float maxDepth, float volume, float waterHeight, float liquideDensity)
//{
//	//hauteur du sol
//	m_maxDepth = maxDepth;
//
//	//volume de l'objet
//	m_volume = volume;
//
//	//Niveau de l'eau
//	m_waterHeight = waterHeight;
//
//	//Densité de l'eau
//	m_liquideDensity = liquideDensity;
//}
//
//void ParticleBuoyancy::UpdateForce(Particule* particle, float deltaTime)
//{
//	float depth = particle->GetPosition().y;
//
//	//si l'objet est hors de l' eau -> aucune force
//	if (depth >= m_waterHeight + m_maxDepth) return;
//
//	Vector3D force(0, 0, 0);
//
//	//si l'objet est totalement dans l' eau -> force constante volume*densité de l'eau
//	if (depth <= m_waterHeight - m_maxDepth)
//	{
//		force.y = m_liquideDensity * m_volume;
//		particle->AddForce(force);
//		return;
//	}
//	//si l'objet est en partie dans l' eau -> on fait en fonction de la prtie du volume qui est dans l'eau (pourcentage pour faire simple)
//	force.y = m_liquideDensity * m_volume * abs(depth - m_maxDepth - m_waterHeight) / 2 * m_maxDepth;
//	particle->AddForce(force);
//}