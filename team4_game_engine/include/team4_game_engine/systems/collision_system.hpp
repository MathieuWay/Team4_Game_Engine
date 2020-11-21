#pragma once
#include <team4_game_engine/common.hpp>
#include <team4_game_engine/systems/system.hpp>

namespace team4_game_engine::systems {
	class CollisionSystemImpl;
	class CollisionSystem : public ISystem {
	public:
		CollisionSystem(int resolveIteration);
		virtual ~CollisionSystem();

		CollisionSystem(const CollisionSystem& other) = delete;
		CollisionSystem(CollisionSystem&& other) = delete;

		virtual void Update(std::chrono::milliseconds deltatime, engine::World& world) override;
	private:
		std::unique_ptr<CollisionSystemImpl> m_impl;
	};
}