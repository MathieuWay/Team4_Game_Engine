#include <team4_game_engine/include/team4_game_engine/engine/physics/Contact.hpp>
#include <iostream>

namespace team4_game_engine::physics
{
    Contact::Contact(Vector3D _contactPoint,  float _penetration) {
        this->contactPoint = _contactPoint;
        this->contactNormal = _contactPoint.normalize();
        this->penetration = _penetration;
    }

    Contact::~Contact()
    {
    }


    
}
