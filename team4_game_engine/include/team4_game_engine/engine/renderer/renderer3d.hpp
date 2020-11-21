#pragma once
#include <team4_game_engine/common.hpp>
#include <team4_game_engine/engine/renderer/mesh.hpp>

namespace team4_game_engine::renderer {
	class Renderer3D {
	public:
		static Renderer3D& Instance() {
			static Renderer3D ms_camera;
			return ms_camera;
		}
		std::vector<std::shared_ptr<Mesh>> ms_meshs;
	private:
		Renderer3D() {};
	};
	//std::vector<std::shared_ptr<Mesh>> MeshManager::ms_meshs = std::vector<std::shared_ptr<Mesh>>();
}