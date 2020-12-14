#pragma once
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
using namespace team4_game_engine::engine::mathematics;

namespace team4_game_engine::physics
{
    class Contact
    {
    public:
        
        Vector3D contactPoint;
        Vector3D contactNormal;
        float penetration;

        Contact(Vector3D _contactPoint, float _penetration);
        ~Contact();
    };
}