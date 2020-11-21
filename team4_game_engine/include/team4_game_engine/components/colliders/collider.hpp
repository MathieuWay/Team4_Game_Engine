#pragma once

namespace team4_game_engine::components {
	enum class Shape {
		Sphere,
		Box
	};
	class ShapeData{};
	class Collider {
	public:
		virtual Shape GetShape() { return shape; }
	protected:
		Shape shape;
	};
}