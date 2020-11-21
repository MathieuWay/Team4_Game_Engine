#pragma once
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
#include <team4_game_engine/serialization/input_memory_stream.hpp>
#include <team4_game_engine/serialization/output_memory_stream.hpp>

namespace team4_game_engine::components {
	class Position{
	public:
		engine::mathematics::Vector3D local;
		Position(){}
		Position(float _x, float _y, float _z) {
			local.x = _x;
			local.y = _y;
			local.z = _z;
		}

		void SetPosition(float _x, float _y, float _z) {
			local.x = _x;
			local.y = _y;
			local.z = _z;
		}

		void Write(serialization::OutputMemoryStream& stream) {
			stream.Write(local.x);
			stream.Write(local.y);
			stream.Write(local.z);
		}

		void Read(serialization::InputMemoryStream& stream) {
			local.x = stream.Read<float>();
			local.y = stream.Read<float>();
			local.z = stream.Read<float>();
		}

		void OnInspectorGUI() {
			if (ImGui::CollapsingHeader("Position")) {
				ImGui::DragFloat3("Local Position", &local.x, 0.1f);
			}
		};
	};
}