#pragma once
#include <memory>
#include <vector>
#include <spdlog/spdlog.h>
#include <team4_game_engine/engine/entity.hpp>
#include <team4_game_engine/engine/renderer/buffer_layout.hpp>
#include <team4_game_engine/engine/renderer/shader.hpp>
#include <team4_game_engine/serialization/output_memory_stream.hpp>

namespace team4_game_engine::renderer {
	struct Draw_Mode {
		int mode;
		const char* name;
	};

	static const Draw_Mode Draw_Modes[] = {
		{GL_POINTS, "Points"},
		{GL_LINES, "Lines"},
		{GL_LINE_STRIP, "Lines Strip"},
		{GL_TRIANGLES, "Triangles"},
		{GL_TRIANGLE_STRIP, "Triangles Strip"}
	};

	class Mesh
	{
	public:
		Mesh(std::string name, std::vector<float> vertexData, std::vector<uint32_t> indices, const BufferLayout vertexLayout, const BufferLayout instanceLayout, Shader* shader, int drawMode);
		~Mesh();

		std::string m_name;
		int m_drawMode = 0;
		std::vector<engine::Entity*> instanceReferences;
		void UpdateInstanceBuffer();
		void AddInstance(engine::Entity* entity);
		void RemoveInstance(engine::Entity entity);
		void Draw(glm::mat4 projection, glm::mat4 view);

	private:
		uint32_t VAO, VBO, EBO;
		Shader* program;
		uint32_t verticesCount = 0;
		uint32_t indicesCount = 0;
		std::vector<unsigned int> indices;
		const BufferLayout m_instanceLayout;
		uint32_t InstanceBuffer;
		uint32_t instanceCount;
		// Applique le layout défini dans le buffer rattaché
		void ApplyLayout(const BufferLayout& layout, uint32_t& attributeIndex, int divider);
	};
}

