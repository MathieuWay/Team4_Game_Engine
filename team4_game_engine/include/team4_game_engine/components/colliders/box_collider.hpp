#pragma once
#include <team4_game_engine/components/colliders/collider.hpp>
#include <glm/glm.hpp>

namespace team4_game_engine::components{
	class BoxData : public ShapeData {
	public:
		BoxData(Vector3D _min, Vector3D _max) : min(_min), max(_max) {}
		Vector3D min;
		Vector3D max;
	};
	class BoxCollider : public Collider {
	public:
		BoxData shapeData;
		BoxCollider(Vector3D size) :
			shapeData(BoxData(
				Vector3D(-size.x / 2, -size.y / 2, -size.z / 2),
				Vector3D(size.x / 2, size.y / 2, size.z / 2)))
		{
			shape = Shape::Box;
		}
		BoxData GetBoxData() {
			return shapeData;
		}
	};
}