#include <team4_game_engine/engine/physics/CollisionData.hpp>
#include <spdlog/spdlog.h>
#include <iostream>

namespace team4_game_engine::physics 
{
	int CollisionData::remainContacts = 4;

	CollisionData::CollisionData(std::vector<Contact*> _contact, RigidBody& _Prim1, RigidBody& _Prim2) : contact(_contact), Prim1(_Prim1), Prim2(_Prim2)
	{
	}

	CollisionData::~CollisionData()
	{
	}

	void CollisionData::ResolveContact()
	{
		if (Prim1.mass != 0) {
			spdlog::info("Il y a {0} points de contact dans Prim1:", contact.size());
			for (int i = 0; i < contact.size(); i++) {
				spdlog::info("Point de contact: {0}", Matrix4::worldToLocal(contact[i]->contactPoint, Prim1.transforMatrix).GetVectorData());
				spdlog::info("Normale du point de contact: {0}", contact[i]->contactNormal.GetVectorData());
				spdlog::info("Le facteur d'interp¨¦n¨¦tration est: {0}", contact[i]->penetration);
			}
				spdlog::info("Le coefficient de restitution de Prim1 est: {0}", Prim1.restitutionCoef);
		}
		if (Prim2.mass != 0) {
			spdlog::info("Il y a {0} points de contact dans Prim2:", contact.size());
			for (int i = 0; i < contact.size(); i++) {
				spdlog::info("Point de contact: {0}", Matrix4::worldToLocal(contact[i]->contactPoint, Prim2.transforMatrix).GetVectorData());
				spdlog::info("Normale du point de contact: {0}", contact[i]->contactNormal.GetVectorData());
				spdlog::info("Le facteur d'interp¨¦n¨¦tration est: {0}", contact[i]->penetration);
			}
			spdlog::info("Le coefficient de restitution de Prim1 est: {0}", Prim2.restitutionCoef);
		}
	}
}