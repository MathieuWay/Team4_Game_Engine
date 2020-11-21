#include <team4_game_engine/engine/physics/collision_resolver.hpp>

namespace team4_game_engine::physics {
	CollisionResolver::CollisionResolver(unsigned int iterations) {
		m_iterations = iterations;
		iterationsUsed = 0;
	}

	void CollisionResolver::setIterations(unsigned int iterations) {
		m_iterations = iterations;
	}

	void CollisionResolver::resolveCollisions(std::vector<Collision*> collisionArray, float deltatime) {
		iterationsUsed = 0;
		while (iterationsUsed < m_iterations) {
			// recherche la colision avec la plus grande vitesse de rapprochement (la vitesse la plus negative)
			float maxClosingVelocity = FLT_MAX;
			unsigned maxIndex = collisionArray.size();

			for (unsigned i = 0; i < collisionArray.size(); i++) {
				float sepVelocity = collisionArray[i]->calculateSeparateVelocity();
				if (sepVelocity < maxClosingVelocity && (sepVelocity < 0 || collisionArray[i]->penetration > 0)) {
					maxClosingVelocity = sepVelocity;
					maxIndex = i;
				}
			}

			// coupe la boucle si le nombre de collision est atteint 
			if (maxIndex == collisionArray.size()) {
				break;
			}

			// resout la colision
			collisionArray[maxIndex]->resolve(deltatime);
			if (collisionArray[maxIndex]->calculateSeparateVelocity() > 0) {
				collisionArray.erase(collisionArray.begin() + maxIndex);
				if (collisionArray.size() == 0) break;
			}
			iterationsUsed++;
		}
	}
}