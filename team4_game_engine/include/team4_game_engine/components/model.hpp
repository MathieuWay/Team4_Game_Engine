#pragma once
#include <vector>
#include <memory>
#include <team4_game_engine/engine/renderer/mesh.hpp>
//#include <team4_game_engine/engine/renderer/material.hpp>

namespace team4_game_engine::components {
	struct Model {
		std::vector<std::shared_ptr<renderer::Mesh>> meshs;
		//std::vector<Material> m_material;
		void OnInspectorGUI() {
			if (ImGui::CollapsingHeader(ComponentsLabels[ComponentType::Model])) {

			}
		}
	};
}