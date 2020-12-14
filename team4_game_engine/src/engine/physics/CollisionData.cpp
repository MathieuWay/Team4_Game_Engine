#include <team4_game_engine/engine/physics/CollisionData.hpp>
#include <spdlog.h>
#include <iostream>

namespace team4_game_engine::physics 
{
	int CollisionData::remainContact = 4;

	CollisionData::CollisionData(std::vector<Contact*> _contact, Rigidbody& _Prim1, Rigidbody& _Prim2) : Contact(_contact), Prim1(_Prim1), Prim2(_Prim2)
	{
	}

	CollisionData::~CollisionData()
	{
	}

	void CollisionData::ResolveContact()
	{
		if (Prim1.mass != 0) {
			spdlog::info("The Contact point is {}", Matrix4::worldToLocal(contact.contactPoint, Prim1.transformMatrix));
			spdlog::info("The contact point normal is {}", contact.contactNormal);
			spdlog::info("The penetration factor is {}", penetration);
			spdlog::info("The restitution coefficient is {}", Prim1.restitutionCoef);
		}
		if (Prim2.mass != 0) {
			spdlog::info("The Contact point is {}", Matrix4::worldToLocal(contact.contactPoint, Prim2.transformMatrix));
			spdlog::info("The contact point normal is {}", contact.contactNormal);
			spdlog::info("The penetration factor is {}", penetration);
			spdlog::info("The restitution coefficient is {}", Prim2.restitutionCoef);
		}
	}
}