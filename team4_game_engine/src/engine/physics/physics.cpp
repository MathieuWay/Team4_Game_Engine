#include <team4_game_engine/engine/physics/physics.hpp>

namespace team4_game_engine::physics {
	bool Physics::doPhysicsStep = true;
	bool Physics::doCollisionStep = true;
	bool Physics::doNextStep = false;
	bool Physics::useFixedDeltatime = false;
	int Physics::physicsStepPerSecond = 50;
	float Physics::physicsStepTime = (float)1 / (float)50;
}