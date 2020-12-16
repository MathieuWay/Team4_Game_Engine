#pragma once
#include <entt/entt.hpp>
#include <team4_game_engine/components/name.hpp>
#include <team4_game_engine/components/rigidbody.hpp>
#include <team4_game_engine/engine/physics/Contact.hpp>
#include <team4_game_engine/engine/mathematics/matrix4.hpp>
#include <vector>
using namespace team4_game_engine::components;

namespace team4_game_engine::physics 
{
	class CollisionData 
	{
	public:
		//liste de contact entre deux objets
		std::vector<Contact*> contact;
		//premier objet faisant parti de la collison
		entt::entity entity1;
		//deuxieme objet faisant parti de la collision
		entt::entity entity2;

	static int remainContacts;


		CollisionData(std::vector<Contact*> _contact, entt::entity _entity1, entt::entity _entity2);
		~CollisionData();

		//Resolution des collision entre les deux objets
		void ResolveContact();
	};
}