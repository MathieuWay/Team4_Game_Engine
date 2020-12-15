#include <team4_game_engine/engine/physics/Contact.hpp>
#include <iostream>

namespace team4_game_engine::physics
{
    Contact::Contact(Vector3D _contactPoint, Vector3D _contactNormal, float _penetration) {
        this->contactPoint = _contactPoint;
        this->contactNormal = _contactNormal;
        this->penetration = _penetration;
    }

    Contact::~Contact()
    {
    }


    
}
