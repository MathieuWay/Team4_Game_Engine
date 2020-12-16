#pragma once
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
using namespace team4_game_engine::engine::mathematics;

namespace team4_game_engine::components {
	// Cree une sphere de rayon radius a partir du centre de l'objet pour simplifier la detection de collision
	struct BoundingVolume {
		float radius;
	};
}