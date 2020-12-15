#pragma once
#include <entt/entt.hpp>

#include <team4_game_engine/engine/engine.hpp>
#include <team4_game_engine/engine/world.hpp>
#include <team4_game_engine/engine/entity.hpp>
using namespace team4_game_engine::engine;

#include <team4_game_engine/engine/mathematics/vector3d.hpp>
#include <team4_game_engine/engine/mathematics/matrix3.hpp>
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
#include <team4_game_engine/components/colliders/plane_collider.hpp>
#include <team4_game_engine/components/colliders/box_collider.hpp>
#include <team4_game_engine/components/colliders/sphere_collider.hpp>
#include <team4_game_engine/components/boundingVolume.hpp>
using namespace team4_game_engine::components;

namespace team4_game_engine::engine::primitive {

	static Entity* Empty(std::string name, entt::entity parent) {
		std::shared_ptr<World> world = Engine::Instance().GetWorld().lock();
		Entity* entity = new Entity(*world, name);
		entity->GetComponent<Parent>().entity = parent;
		if (parent != entt::null) {
			world->Registry().get<Children>(parent).childrens.push_back(static_cast<entt::entity>(entity->EntityID()));
		}
		float radius = 1.0f;
		entity->AddComponent<components::BoundingVolume>(radius);
		return entity;
	}

	static Entity* Sphere(std::string name, entt::entity parent, int mesh) {
		renderer::Renderer3D& instance = renderer::Renderer3D::Instance();
		Entity* entity = Empty(name, parent);
		entity->AddComponent<components::Color>(1.0f, 1.0f, 1.0f);
		Matrix3 iit = Matrix3();
		iit.data[0] = 2/5 * 1 * 1 ;
		iit.data[4] = 2 / 5 * 1 * 1;
		iit.data[8] = 2 / 5 * 1 * 1;
		auto& rb = entity->AddComponent<RigidBody>(
			new SphereCollider(1),
			1,
			1,
			0.995,
			0.995,
			iit,
			Vector3D(0, -10, 0));

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
		Matrix3 iit = Matrix3();
		iit.data[0] = 1 / 12 * 1 + (1 + 1);
		iit.data[4] = 1 / 12 * 1 + (1 + 1);
		iit.data[8] = 1 / 12 * 1 + (1 + 1);
		auto& rb = entity->AddComponent<RigidBody>(
			new BoxCollider(Vector3D(1.0f, 1.0f, 1.0f)),
			1,
			1,
			0.995,
			0.995,
			iit,
			Vector3D(0, -10, 0));

		auto meshs = std::vector<std::shared_ptr<team4_game_engine::renderer::Mesh>>();
		meshs.push_back(instance.ms_meshs[mesh]);
		entity->AddComponent<Model>(meshs);
		instance.ms_meshs[mesh]->AddInstance(entity);
		return entity;
	}

	static Entity* Plane(std::string name, entt::entity parent, int mesh) {
		renderer::Renderer3D& instance = renderer::Renderer3D::Instance();
		Entity* entity = Empty(name, parent);
		entity->AddComponent<components::Color>(1.0f, 1.0f, 1.0f);
		entity->GetComponent<Scale>() = { 20.f, 1, 20.f };
		Matrix3 iit = Matrix3();
		iit.data[0] = 1 / 12 * 1 + (1 + 1);
		iit.data[4] = 1 / 12 * 1 + (1 + 1);
		iit.data[8] = 1 / 12 * 1 + (1 + 1);
		auto& rb = entity->AddComponent<RigidBody>(
			new PlaneCollider(Vector3D(0.f, 1.f, 0.f)),
			0,
			0.5,
			0,
			0,
			iit,
			Vector3D(0, 0, 0),
			false);

		auto meshs = std::vector<std::shared_ptr<team4_game_engine::renderer::Mesh>>();
		meshs.push_back(instance.ms_meshs[mesh]);
		entity->AddComponent<Model>(meshs);
		instance.ms_meshs[mesh]->AddInstance(entity);
		return entity;
	}
}