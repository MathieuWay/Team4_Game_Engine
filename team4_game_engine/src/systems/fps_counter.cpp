#include <team4_game_engine/systems/fps_counter.hpp>
#include <spdlog/spdlog.h>
#include <team4_game_engine/engine/engine.hpp>
#include <team4_game_engine/engine/world.hpp>

namespace team4_game_engine::systems {
	class FPSCounterImpl {
	public:
		void Update(std::chrono::milliseconds deltatime, engine::World& world) {
			m_frames++;
			m_totalMilli += deltatime;
			if (m_totalMilli >= 1s) {
				spdlog::debug("FPS : {}", m_frames);
				m_frames = 0;
				m_totalMilli = 0ms;
			}
		}
	private:
		int m_frames = 0;
		std::chrono::milliseconds m_totalMilli = 0ms;
	};

	FPSCounter::FPSCounter() {
		m_impl = std::make_unique<FPSCounterImpl>();
	}

	FPSCounter::~FPSCounter(){
		m_impl = nullptr;
	}

	void FPSCounter::Update(std::chrono::milliseconds deltatime, engine::World& world) {
		m_impl->Update(deltatime, world);
	}
}