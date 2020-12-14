#pragma once
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
		RigidBody& Prim1;
		RigidBody& Prim2;

	static int remainContacts;


		CollisionData(std::vector<Contact*> _contact, RigidBody& _Prim1, RigidBody& _Prim2);
		~CollisionData();

		void ResolveContact();
	};
}