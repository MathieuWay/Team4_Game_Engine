#include "..\..\include\team4_game_engine\engine\entity.hpp"
#include <team4_game_engine/engine/entity.hpp>

#include <team4_game_engine/components/name.hpp>
#include <team4_game_engine/components/parent.hpp>
#include <team4_game_engine/components/children.hpp>
#include <team4_game_engine/components/color.hpp>

namespace team4_game_engine::engine {
	uint64_t Entity::ms_lastObjectID = 0;

	Entity::Entity(World& world, const std::string& name) :
		m_world(world),
		m_name(name)
	{
		m_entityID = world.CreateEntity();
		m_ObjectID = ms_lastObjectID++;
		AddComponent<components::Name>(m_name);
		AddComponent<components::Parent>(entt::null);
		AddComponent<components::Children>(std::vector<entt::entity>());
		AddComponent<components::Position>(0.f, 0.f, 0.f);
		AddComponent<components::Rotation>();
		AddComponent<components::Scale>(1.f, 1.f, 1.f);
	}

	Entity::~Entity() {

	}

	void Entity::Write(serialization::OutputMemoryStream& stream) {
		Position().Write(stream);
		Rotation().Write(stream);
		Scale().Write(stream);
	}

	void Entity::Read(serialization::InputMemoryStream& stream) {
		Position().Read(stream);
		Rotation().Read(stream);
		Scale().Read(stream);
	}
}