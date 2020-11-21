#pragma once
#include <team4_game_engine/serialization/input_memory_stream.hpp>
#include <team4_game_engine/serialization/output_memory_stream.hpp>

namespace team4_game_engine::components {
	struct Rotation {
		float x;
		float y;
		float z;
		float w;

		void Write(serialization::OutputMemoryStream& stream) {
			stream.Write(x);
			stream.Write(y);
			stream.Write(z);
			stream.Write(w);
		}

		void Read(serialization::InputMemoryStream& stream) {
			x = stream.Read<float>();
			y = stream.Read<float>();
			z = stream.Read<float>();
			w = stream.Read<float>();
		}

		void OnInspectorGUI() {
			if (ImGui::CollapsingHeader("Rotation")) {
				ImGui::DragFloat4("Local", &x, 0.1f);
			}
		};
	};
}