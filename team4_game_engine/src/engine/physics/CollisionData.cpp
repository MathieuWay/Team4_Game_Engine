#include <team4_game_engine/engine/physics/CollisionData.hpp>
#include <spdlog/spdlog.h>
#include <team4_game_engine/engine/engine.hpp>
#include <team4_game_engine/engine/world.hpp>
#include <iostream>

namespace team4_game_engine::physics 
{
	int CollisionData::remainContacts = 4;

	CollisionData::CollisionData(std::vector<Contact*> _contact, entt::entity _entity1, entt::entity _entity2) : contact(_contact), entity1(_entity1), entity2(_entity2)
	{
	}

	CollisionData::~CollisionData()
	{
	}

	void CollisionData::ResolveContact()
	{
		//affichage des points de contact, des normales, des facteurs d'interpenetrations et des coefficients de restitution pour chaque contact entre deux objets
		//Pour les deux objets si leur masse n'est pas egale a 0
		RigidBody& Prim1 = engine::Engine::Instance().GetWorld().lock()->GetComponent<RigidBody>(entity1);
		RigidBody& Prim2 = engine::Engine::Instance().GetWorld().lock()->GetComponent<RigidBody>(entity2);
		if (Prim1.mass != 0) {
			Name name = engine::Engine::Instance().GetWorld().lock()->GetComponent<Name>(entity1);
			spdlog::info("Il y a {0} points de contact dans {1}:", contact.size(), name);
			for (int i = 0; i < contact.size(); i++) {
				spdlog::info("Point de contact: {0}", contact[i]->contactPoint.GetVectorData());
				spdlog::info("Normale du point de contact: {0}", contact[i]->contactNormal.GetVectorData());
				spdlog::info("Le facteur d'interpenetration est: {0}", contact[i]->penetration);
			}
				spdlog::info("Le coefficient de restitution de {0} est: {1}", name, Prim1.restitutionCoef);
		}
		if (Prim2.mass != 0) {
			Name name = engine::Engine::Instance().GetWorld().lock()->GetComponent<Name>(entity2);
			spdlog::info("Il y a {0} points de contact dans {1}:", contact.size(), name);
			for (int i = 0; i < contact.size(); i++) {
				spdlog::info("Point de contact: {0}", contact[i]->contactPoint.GetVectorData());
				spdlog::info("Normale du point de contact: {0}", contact[i]->contactNormal.GetVectorData());
				spdlog::info("Le facteur d'interpenetration est: {0}", contact[i]->penetration);
			}
			spdlog::info("Le coefficient de restitution de {0} est: {1}", name, Prim2.restitutionCoef);
		}
		for (int i = 0; i < contact.size(); i++)
		{
			delete contact[i];
		}
		contact.clear();
	}
}