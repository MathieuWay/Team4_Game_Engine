#pragma once
#include <team4_game_engine/include/team4_game_engine/components/rigidbody.hpp>
#include <team4_game_engine/include/team4_game_engine/engine/physics/Contact.hpp>
#include <team4_game_engine/include/team4_game_engine/engine/mathematics/matrix4.hpp>
#include <vector>

namespace team4_game_engine::physics 
{
	class CollisionData 
	{
	public:
		std::vector<Contact*> contact;
		Rigidbody& Prim1;
		Rigidbody& Prim2;

	static int remainContacts;


		CollisionData(std::vector<Contact*> _contact, Rigidbody& _Prim1, Rigidbody& _Prim2);
		~CollisionData();

		void ResolveContact();
	};
}