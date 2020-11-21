#pragma once
#include <team4_game_engine/common.hpp>
#include <team4_game_engine/engine/physics/collision.hpp>

namespace team4_game_engine::physics {
	class CollisionResolver {

	protected:
		/*
		contient le nombre d'iterations autorisees pour traiter les colisions
		*/
		unsigned m_iterations;

		/*
		contient le nombre d'iterations deja utilisees
		*/
		unsigned iterationsUsed;

	public:
		/*
			cree un nouveau solveur de colision
		*/
		CollisionResolver(unsigned int iterations);

		/*
			set le nombre d'iterations
		*/
		void setIterations(unsigned int iterations);

		/*
			resout les colisions pour les diffents types de colision
		*/
		void resolveCollisions(std::vector<Collision*> collisionArray, float deltatime);

	};
}