#pragma once

namespace team4_game_engine::physics {
	class Physics {
	public:
		static bool doPhysicsStep;
		static bool doNextStep;
		static bool useFixedDeltatime;
		static int physicsStepPerSecond;
		static float physicsStepTime;
	};
}