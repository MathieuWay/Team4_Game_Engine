#pragma once
#include <map>
namespace team4_game_engine::components {
	enum class ComponentType {
		NotInstanced,
		Name,
		Position,
		Rotation,
		Scale,
		Color,
		RigidBody,
		Model,
		BoxCollider,
		SphereCollider,
		Count
	};
	static std::map<ComponentType, const char*> ComponentsLabels = {
		{ ComponentType::Name, "Name" },
		{ ComponentType::Color, "Position" },
		{ ComponentType::Color, "Rotation" },
		{ ComponentType::Color, "Scale" },
		{ ComponentType::Color, "Color" },
		{ ComponentType::Model, "Model" },
		{ ComponentType::RigidBody, "RigidBody" },
		{ ComponentType::BoxCollider, "Box Collider" },
		{ ComponentType::SphereCollider, "Sphere Collider" }
	};
}