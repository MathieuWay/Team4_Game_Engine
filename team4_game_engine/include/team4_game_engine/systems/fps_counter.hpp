#pragma once
#include <team4_game_engine/common.hpp>
#include <team4_game_engine/systems/system.hpp>

namespace team4_game_engine::systems {
	class FPSCounterImpl;
	class FPSCounter : public ISystem {
	public:
		FPSCounter();
		virtual ~FPSCounter();

		FPSCounter(const FPSCounter& other) = delete;
		FPSCounter(FPSCounter&& other) = delete;

		virtual void Update(std::chrono::milliseconds deltatime, engine::World& world) override;
	private:
		std::unique_ptr<FPSCounterImpl> m_impl;
	};
}