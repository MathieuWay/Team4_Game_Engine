#pragma once
#include <team4_game_engine/components/colliders/collider.hpp>
#include <glm/glm.hpp>

namespace team4_game_engine::components {
	class SphereData : public ShapeData {
	public:
		SphereData(float _radius) : radius(_radius) {}
		float radius;
	};
	class SphereCollider : public Collider {
	public:
		SphereData shapeData;
		SphereCollider(float size) : shapeData(SphereData(size / 2)) {
			shape = Shape::Sphere;
		}
		SphereData GetSphereData(){
			return shapeData;
		}
	};
}