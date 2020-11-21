#pragma once
#include <entt/entt.h>
namespace team_game_engine::components {
	enum Forces {
		gravity,
		drag,
		basicSpring,
		anchoredSpring
	};
	class ParticleForceGenerator
	{
	public:

		//deuxième particule de la force, si il n'y en a pas elle est egale à nullptr
		World& world;
		entt:entity other = -1;
		virtual void UpdateForce(entt:entity entity, float deltaTime) = 0;

	};
}