#pragma once
#include <team4_game_engine/components/colliders/collider.hpp>

namespace team4_game_engine::components {
	class PlaneData : public ShapeData {
	public:
		PlaneData(Vector3D _normal) : normal(_normal) {}
		Vector3D normal;
	};
	class PlaneCollider : public Collider {
	public:
		PlaneData shapeData;
		PlaneCollider(Vector3D normale) :
			shapeData(PlaneData(normale))
		{
			shape = Shape::Plane;
		}
		PlaneData GetPlaneData() {
			return shapeData;
		}

		void SetPlaneData(PlaneData planeData) {
			shapeData = planeData;
		}
	};
}