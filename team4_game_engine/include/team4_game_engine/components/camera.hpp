#pragma once
#include <glm/glm.hpp>

namespace team4_game_engine::components {
	struct Camera
	{
		glm::mat4 view;
		glm::vec3 backgroundColor;
		float nearClip;
		float farClip;
		float FOV;
	};
}