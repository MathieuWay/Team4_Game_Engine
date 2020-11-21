#include <team4_game_engine/engine/renderer/mesh.hpp>
#include <glad/glad.h>

#include <team4_game_engine/engine/mathematics/vector3d.hpp>
#include <team4_game_engine/engine/mathematics/matrix4.hpp>
using namespace team4_game_engine::engine::mathematics;

#include <team4_game_engine/components/position.hpp>
#include <team4_game_engine/components/color.hpp>
#include <team4_game_engine/engine/engine.hpp>
#include <team4_game_engine/engine/world.hpp>
#include <glm/gtx/quaternion.hpp>
namespace team4_game_engine::renderer {
	Mesh::Mesh(std::string name, std::vector<float> vertexData, std::vector<uint32_t> indices, const BufferLayout vertexLayout, const BufferLayout instanceLayout, Shader* shader, int drawMode) : m_name(name), /*instanceReferences(0),*/ m_instanceLayout(instanceLayout), program(shader), m_drawMode(drawMode)
	{
		uint32_t attribCount = 0;
		// Gen VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Gen VBO
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
		verticesCount = sizeof(float) * vertexData.size() / vertexLayout.GetStride();
		ApplyLayout(vertexLayout, attribCount, 0);

		// Gen EBO
		if (indices.size() > 0) {
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);
			indicesCount = indices.size();
		}
		else {
			EBO = 0;
		}

		// Gen instanceBuffer
		glGenBuffers(1, &InstanceBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, InstanceBuffer);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
		ApplyLayout(instanceLayout, attribCount, 1);
		instanceCount = 0;
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void Mesh::UpdateInstanceBuffer() {
		serialization::OutputMemoryStream instancesData(m_instanceLayout.GetStride() * instanceReferences.size());
		for (auto& entity : instanceReferences) {
			for (auto element : m_instanceLayout) {
				switch (element.componentType)
				{
				case components::ComponentType::Transform: {
					//Translation
					components::Position pos = entity->Position();
					glm::mat4 translationMatrix = glm::mat4(1.0f);
					translationMatrix = glm::translate(translationMatrix, glm::vec3(pos.local.x, pos.local.y, pos.local.z));

					//Rotation
					components::Rotation rot = entity->Rotation();
					glm::mat4 rotationMatrix = glm::toMat4(glm::quat(rot.w, rot.i, rot.j, rot.k));

					//Scale
					components::Scale scale = entity->Scale();
					glm::mat4 scaleMatrix = glm::mat4(1.0f);
					scaleMatrix = glm::scale(scaleMatrix, glm::vec3(scale.x, scale.y, scale.z));

					glm::mat4 transform = translationMatrix * rotationMatrix * scaleMatrix;
					/*const float* ptr = (const float*)glm::value_ptr(transform);
					for (int i = 0; i < 16; i++) {
						float value = ptr[i];
						instancesData.Write(value);
					}*/
					instancesData.Write((char*)glm::value_ptr(transform), sizeof(glm::mat4));
					break; }
				case components::ComponentType::Position:
					entity->Position().Write(instancesData);
					break;
				case components::ComponentType::Rotation:
					entity->Rotation().Write(instancesData);
					break;
				case components::ComponentType::Scale:
					entity->Scale().Write(instancesData);
					break;
				case components::ComponentType::Color:
					entity->GetComponent<components::Color>().Write(instancesData);
					break;
				default:
					assert(1 == 0);
					break;
				}
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, InstanceBuffer);
		 if (instanceReferences.size() > 0) {
		 	glBufferData(GL_ARRAY_BUFFER, instancesData.Size(), instancesData.Data(), GL_DYNAMIC_DRAW);
		 }
		 else {
			glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
		 }
		instanceCount = instanceReferences.size();
	}

	 void Mesh::AddInstance(engine::Entity* entity) {
	 	instanceReferences.push_back(entity);
	 }

	 void Mesh::RemoveInstance(engine::Entity entity) {
	 	int i = 0;
	 	while (i < instanceReferences.size()) {
	 		if (instanceReferences[i]->EntityID() == entity.EntityID()) {
	 			instanceReferences.erase(instanceReferences.begin() + i);
	 			break;
	 		}
	 		i++;
	 	}
	 }

	// Applique le layout défini dans le buffer rattaché
	// gestion particulière des types Mat3 / Mat4 car les vertex attribute sont limités à 4 valeurs max
	// Mat3 (3*3 valeurs) & Mat4 (4*4 valeurs) sont donc gérés différement pour pouvoir les utilisés comme attribut
	void Mesh::ApplyLayout(const BufferLayout& layout, uint32_t& attributeIndex, int divider = 0)
	{
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case ShaderDataType::Mat3:
				for (uint32_t i = 0; i < 3; i++)
				{
					int offset = element.Offset + (i * 3 * 3);
					glEnableVertexAttribArray(attributeIndex);
					glVertexAttribPointer(
						attributeIndex,
						element.GetComponentCount(),
						ShaderDataTypeBaseType(element.Type),
						element.Normalize ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(offset)
					);
					if (divider > 0)
						glVertexAttribDivisor(attributeIndex, divider);
					attributeIndex++;
				}
				break;
			case ShaderDataType::Mat4:
				for (uint32_t i = 0; i < 4; i++)
				{
					int offset = element.Offset + (i * 4 * 4);
					glEnableVertexAttribArray(attributeIndex);
					glVertexAttribPointer(
						attributeIndex,
						element.GetComponentCount(),
						ShaderDataTypeBaseType(element.Type),
						element.Normalize ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(offset)
					);
					if (divider > 0)
						glVertexAttribDivisor(attributeIndex, divider);
					attributeIndex++;
				}
				break;
			default:
				glEnableVertexAttribArray(attributeIndex);
				glVertexAttribPointer(
					attributeIndex,
					element.GetComponentCount(),
					ShaderDataTypeBaseType(element.Type),
					element.Normalize ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset
				);
				if (divider > 0)
					glVertexAttribDivisor(attributeIndex, divider);
				attributeIndex++;
			}
		}
	}

	// Affiche les données du vertex array à l'écran selon sa configuration:
	// - avec ou non un index buffer
	// - avec ou non un instance buffer
	// utilisation des fonctions appropriées selon la situation
	// met à jour l'uniform de la projection et de vue du shader attaché au vertex array
	void Mesh::Draw(glm::mat4 projection, glm::mat4 view)
	{
		program->Use();
		program->setMat4("view", view);
		program->setMat4("projection", projection);
		glBindVertexArray(VAO);
		if (instanceCount > 0) {
			if (EBO != 0) {
				glDrawElementsInstanced(Draw_Modes[m_drawMode].mode, indicesCount, GL_UNSIGNED_INT, nullptr, instanceCount);
			}
			else {
				glDrawArraysInstanced(Draw_Modes[m_drawMode].mode, 0, verticesCount, instanceCount);
			}
		}
	}
}