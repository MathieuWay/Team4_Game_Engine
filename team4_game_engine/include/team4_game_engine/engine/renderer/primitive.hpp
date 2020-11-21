#pragma once
#include <team4_game_engine/engine/renderer/mesh.hpp>
#include <team4_game_engine/engine/renderer/shader.hpp>
using namespace team4_game_engine::renderer;
static Mesh Cube(Shader* shader) {
	float vertices[] = {
			-0.5f,  0.5f,  0.5f, 0, 1, 1,
			 0.5f,  0.5f,  0.5f, 1, 1, 1,
			-0.5f, -0.5f,  0.5f, 0, 0, 1,
			 0.5f, -0.5f,  0.5f, 1, 0, 1,

			 0.5f, -0.5f, -0.5f, 1, 0, 0,
			 0.5f,  0.5f, -0.5f, 1, 1, 0,

			-0.5f, -0.5f, -0.5f, 0, 0, 0,
			-0.5f,  0.5f, -0.5f, 0, 1, 0
	};

	uint32_t indices[] = {  // Triangles Strip indices
		0, 1,
		2, 3,
		4, 1,
		5, 0,
		7, 2,
		6, 4,
		7, 5
	};
	return Mesh(
		"Cube",
		std::vector<float>(vertices, vertices + sizeof vertices / sizeof vertices[0]),
		std::vector<uint32_t>(indices, indices + sizeof indices / sizeof indices[0]),
		{ {ShaderDataType::Float3, "a_Pos"}, {ShaderDataType::Float3, "a_Color"} },
		{ {ShaderDataType::Float3, "a_Offset"} },
		shader,
		4
	);
}