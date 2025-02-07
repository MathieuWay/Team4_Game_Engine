﻿#pragma once
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
using namespace team4_game_engine::engine::mathematics;

namespace team4_game_engine::physics
{
    class Contact
    {
    public:
        
		//Point de contact de coordonnee monde
        Vector3D contactPoint;

		//normale au point de contact en coordonnee monde
        Vector3D contactNormal;

		//pénétration de l'objet par rapport à cette normale
        float penetration;

        Contact(Vector3D _contactPoint, Vector3D contactNormal, float _penetration);
        ~Contact();
    };
}