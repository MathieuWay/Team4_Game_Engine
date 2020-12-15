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
		std::vector<Contact*> contact;
		entt::entity entity1;
		entt::entity entity2;

	static int remainContacts;


		CollisionData(std::vector<Contact*> _contact, entt::entity _entity1, entt::entity _entity2);
		~CollisionData();

		void ResolveContact();
	};
}