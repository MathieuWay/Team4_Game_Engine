#pragma once
#include <team4_game_engine/common.hpp>
#include <team4_game_engine/systems/system.hpp>

namespace team4_game_engine::systems {
	class PhysicsSystemImpl;
	class PhysicsSystem : public ISystem {
	public:
		PhysicsSystem();
		virtual ~PhysicsSystem();

		PhysicsSystem(const PhysicsSystem& other) = delete;
		PhysicsSystem(PhysicsSystem&& other) = delete;

		virtual void Update(std::chrono::milliseconds deltatime, engine::World& world) override;
	private:
		std::unique_ptr<PhysicsSystemImpl> m_impl;
	};
}