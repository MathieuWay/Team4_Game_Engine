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
		float GetDistance() {
			return distance;
		}
		void SetDistance(float dist) {
			float* mat = glm::value_ptr(m_component.view);
			m_component.view = glm::translate(m_component.view, glm::vec3(mat[2], mat[6], mat[10]) * -(dist - distance));
			distance = dist;
		}

		//Zooming / Dezooming
		void OnScroll(double delta) {
			float deltaValue = (float)-delta * 0.1f;
			float newDistance = distance + deltaValue;
			if (newDistance < 0.5f) {
				newDistance = 0.5f;
			}
			SetDistance(newDistance);
			//if (newDistance > 0.5f) {
			//	float* mat = glm::value_ptr(Scene::Instance()->view);
			//	//https://i.stack.imgur.com/KW71r.jpg
			//	Scene::Instance()->view = glm::translate(Scene::Instance()->view, glm::vec3(mat[2], mat[6], mat[10]) * -deltaValue);
			//}
		}
	private:
		SceneView() {
			m_entityID = -1;
			distance = 10;
			m_component = { glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, 100, 70 };
			m_component.view = glm::rotate(m_component.view, glm::radians(15.0f), glm::vec3(1, 0, 0));
			float* mat = glm::value_ptr(m_component.view);
			m_component.view = glm::translate(m_component.view, glm::vec3(mat[2], mat[6], mat[10]) * -distance);
			m_projection = glm::mat4();
		};
		uint32_t m_entityID;
		components::Camera m_component;
		glm::mat4 m_projection;
		float distance;
	};
}