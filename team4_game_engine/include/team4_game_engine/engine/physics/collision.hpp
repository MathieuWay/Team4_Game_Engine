#pragma once
#include <entt/entt.hpp>
#include <team4_game_engine/components/rigidbody.hpp>
using namespace team4_game_engine::components;
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
using namespace team4_game_engine::engine::mathematics;

namespace team4_game_engine::physics {
	class Collision
	{
	public:
		/*
		contient les deux particules en contact si la particule est en contact avec le sol paticule[1] = NULL
		*/
		entt::entity entities[2];
		RigidBody* rigidBodies[2];

		/*
		contient la le coefficient de restitution pour simuler l'elasticite de la collision
		*/
		float restitution;

		/*
		Normal au point de contact
		*/
		Vector3D contactNormal;

		/*
		contient la profondeur de penetration aux contacts des particules
		*/
		float penetration;

		/*
			contient le mouvement que les particules doivent réaliser pour que penetration <= 0
		*/
		Vector3D rigidbodyMove[2];

		//protected:

		Collision(entt::entity _entities[2], RigidBody* _rigidBodies[2], float res, float pen, Vector3D contNorm);

		/*
			fait appelle aux fonctions resolveVelocity et resolveInterpenetration
		*/
		void resolve(float duration);

		/*
			calcul la vitesse de separation des ou de la particule concerne par la colision
		*/
		float calculateSeparateVelocity() const;

	private:
		/*
			resout les problemes de colision
		*/
		void resolveVelocity(float duration);

		/*
			resout les problemes d'interpenetration
		*/
		void resolveInterpenetration(float duration);
	};
}

