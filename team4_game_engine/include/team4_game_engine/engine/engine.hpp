#pragma once
#include <team4_game_engine/common.hpp>

namespace team4_game_engine::engine {
	class EngineImpl;
	class World;
	class Engine {
	public:
		static Engine& Instance() {
			static Engine ms_engine;
			return ms_engine;
		}
		~Engine();

		Engine(const Engine& other) = delete;
		Engine(Engine&& other) = delete;

		void ParseCommandeLine(int argc, char* argv[]);
		void Startup();
		void Run();
		void Stop();

		std::weak_ptr<World> GetWorld();
	private:
		Engine();
		std::unique_ptr<EngineImpl> m_impl;
	};

}