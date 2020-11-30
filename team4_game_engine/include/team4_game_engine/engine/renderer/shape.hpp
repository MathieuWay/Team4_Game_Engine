#pragma once
#include <team4_game_engine/engine/renderer/mesh.hpp>
namespace team4_game_engine::renderer::shape {
	static Mesh* GenerateSphere(int sectorCount, int stackCount, float radius) {
		std::vector<float> vertices(0);

		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
		float s, t;                                     // vertex texCoord
		constexpr float PI = glm::pi<float>();
		float sectorStep = 2 * PI / sectorCount;
		float stackStep = PI / stackCount;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stackCount; ++i)
		{
			stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
			xy = radius * cosf(stackAngle);             // r * cos(u)
			z = radius * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sectorCount; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
			}
		}
		// generate CCW index list of sphere triangles
		std::vector<uint32_t> indices;
		int k1, k2;
		for (int i = 0; i < stackCount; ++i)
		{
			k1 = i * (sectorCount + 1);     // beginning of current stack
			k2 = k1 + sectorCount + 1;      // beginning of next stack

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}

		const BufferLayout sphere_Vertexlayout = { {ShaderDataType::Float3, "a_Pos", components::ComponentType::NotInstanced}, {ShaderDataType::Float3, "a_Color", components::ComponentType::NotInstanced} };
		const BufferLayout sphere_Instancelayout = { {ShaderDataType::Mat4, "model", components::ComponentType::Transform} };
		Shader* sphere_shader = new Shader("shader.vs", "shader.fs");
		// Push VAO to VAO List
		return new Mesh(
			"Sphere",
			vertices,
			indices,
			sphere_Vertexlayout,
			sphere_Instancelayout,
			sphere_shader,
			4
		);
	}
}
