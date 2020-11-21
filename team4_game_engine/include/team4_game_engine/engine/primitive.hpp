#pragma once
#include <entt/entt.hpp>

#include <team4_game_engine/engine/engine.hpp>
#include <team4_game_engine/engine/world.hpp>
#include <team4_game_engine/engine/entity.hpp>
using namespace team4_game_engine::engine;

#include <team4_game_engine/engine/mathematics/vector3d.hpp>
using namespace team4_game_engine::engine::mathematics;

#include <team4_game_engine/components/name.hpp>
#include <team4_game_engine/components/parent.hpp>
#include <team4_game_engine/components/children.hpp>
#include <team4_game_engine/components/position.hpp>
#include <team4_game_engine/components/rotation.hpp>
#include <team4_game_engine/components/scale.hpp>
#include <team4_game_engine/components/rigidbody.hpp>
#include <team4_game_engine/components/color.hpp>
#include <team4_game_engine/components/model.hpp>
#include <team4_game_engine/components/colliders/box_collider.hpp>
#include <team4_game_engine/components/colliders/sphere_collider.hpp>
using namespace team4_game_engine::components;

namespace team4_game_engine::engine::primitive {

	static Entity* Empty(std::string name, entt::entity parent) {
		std::shared_ptr<World> world = Engine::Instance().GetWorld().lock();
		Entity* entity = new Entity(*world, name);
		entity->GetComponent<Parent>().entity = parent;
		if (parent != entt::null) {
			world->Registry().get<Children>(parent).childrens.push_back(static_cast<entt::entity>(entity->EntityID()));
		}
		return entity;
	}

	static Entity* Sphere(std::string name, entt::entity parent, int mesh) {
		renderer::Renderer3D& instance = renderer::Renderer3D::Instance();
		Entity* entity = Empty(name, parent);
		entity->AddComponent<components::Color>(1.0f, 1.0f, 1.0f);
		auto& rb = entity->AddComponent<RigidBody>(
			new SphereCollider(1),
			1,
			1,
			1,
			Vector3D(0, -10, 0),
			true);

		auto meshs = std::vector<std::shared_ptr<team4_game_engine::renderer::Mesh>>();
		meshs.push_back(instance.ms_meshs[mesh]);
		entity->AddComponent<Model>(meshs);
		instance.ms_meshs[mesh]->AddInstance(entity);
		return entity;
	}

	static Entity* Cube(std::string name, entt::entity parent, int mesh) {
		renderer::Renderer3D& instance = renderer::Renderer3D::Instance();
		Entity* entity = Empty(name, parent);
		entity->AddComponent<components::Color>(1.0f, 1.0f, 1.0f);
		auto& rb = entity->AddComponent<RigidBody>(
			new BoxCollider(Vector3D(1.0f, 1.0f, 1.0f)),
			1,
			1,
			1,
			Vector3D(0, -10, 0),
			true);

		auto meshs = std::vector<std::shared_ptr<team4_game_engine::renderer::Mesh>>();
		meshs.push_back(instance.ms_meshs[mesh]);
		entity->AddComponent<Model>(meshs);
		instance.ms_meshs[mesh]->AddInstance(entity);
		return entity;
	}
}