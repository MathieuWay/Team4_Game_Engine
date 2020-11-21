#pragma once
#include <team4_game_engine/common.hpp>
#include <team4_game_engine/engine/world.hpp>

namespace team4_game_engine::systems {
	class ISystem {
	public:
		bool isActive = true;
		virtual void BeginFrame() { };
		virtual void EndFrame() { };
		virtual void Update(std::chrono::milliseconds deltatime, engine::World& world) = 0;
	};
}