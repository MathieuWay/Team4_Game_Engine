#pragma once
#include <team4_game_engine/serialization/input_memory_stream.hpp>
#include <team4_game_engine/serialization/output_memory_stream.hpp>

namespace team4_game_engine::components {
	struct Color {
		float r;
		float g;
		float b;

		void Write(serialization::OutputMemoryStream& stream) {
			stream.Write(r);
			stream.Write(g);
			stream.Write(b);
		}

		void Read(serialization::InputMemoryStream& stream) {
			r = stream.Read<float>();
			g = stream.Read<float>();
			b = stream.Read<float>();
		}

		void OnInspectorGUI() {
			if (ImGui::CollapsingHeader("Color")) {
				ImGui::ColorEdit3("Color", &r);
			}
		};
	};
}