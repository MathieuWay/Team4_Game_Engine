#pragma once
#include <team4_game_engine/engine/entity.hpp>
#include <team4_game_engine/components/camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace team4_game_engine::engine {
	class SceneView {
	public:
		static SceneView& Instance() {
			static SceneView ms_view;
			return ms_view;
		}

		components::Camera& GetMainCameraComponent() {
			return m_component;
		}

		void UpdateProjection(float width, float height) {
			if(height > 0)
				m_projection = glm::perspective(glm::radians(m_component.FOV), (float)width / (float)height, m_component.nearClip, m_component.farClip);
		}
		glm::mat4 GetProjection() {
			return m_projection;
		}
	private:
		SceneView() {
			m_entityID = -1;

			glm::mat4 view = glm::mat4(1.0f);
			float* mat = glm::value_ptr(view);
			view = glm::rotate(view, glm::radians(15.0f), glm::vec3(1, 0, 0));
			view = glm::translate(view, glm::vec3(mat[2], mat[6], mat[10]) * -10.0f);

			m_component = { view, glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, 100, 70 };
			m_projection = glm::mat4();
		};
		uint32_t m_entityID;
		components::Camera m_component;
		glm::mat4 m_projection;
	};
}