#pragma once
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
#include <team4_game_engine/serialization/input_memory_stream.hpp>
#include <team4_game_engine/serialization/output_memory_stream.hpp>

namespace team4_game_engine::components {
	struct Scale {
		float x;
		float y;
		float z;

		void Write(serialization::OutputMemoryStream& stream) {
			stream.Write(x);
			stream.Write(y);
			stream.Write(z);
		}

		void Read(serialization::InputMemoryStream& stream) {
			x = stream.Read<float>();
			y = stream.Read<float>();
			z = stream.Read<float>();
		}

		void OnInspectorGUI() {
			if (ImGui::CollapsingHeader("Scale")) {
				ImGui::DragFloat3("Local", &x, 0.1f);
			}
		};
	};
}