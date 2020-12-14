#pragma once
#include <cstdint>
#include <team4_game_engine/engine/world.hpp>
#include <team4_game_engine/serialization/input_memory_stream.hpp>
#include <team4_game_engine/serialization/output_memory_stream.hpp>

#include <team4_game_engine/components/position.hpp>
#include <team4_game_engine/components/rotation.hpp>
#include <team4_game_engine/components/scale.hpp>
#include <team4_game_engine/components/boundingVolume.hpp>

namespace team4_game_engine::engine {
	struct EntityState {
		components::Position& m_pos;
		components::Rotation& m_rot;
		components::Scale& m_scale;

		EntityState(components::Position& pos, components::Rotation& rot, components::Scale& scale) : m_pos(pos), m_rot(rot), m_scale(scale) { }
	};
	class Entity {
	public:
		explicit Entity(World& world, const std::string& name);
		Entity(const Entity& other) = default;
		//Entity& operator=(const Entity& other);
		~Entity();

		virtual void Write(serialization::OutputMemoryStream& stream);
		virtual void Read(serialization::InputMemoryStream& stream);

		uint32_t EntityID() const { return m_entityID; }
		uint64_t ObjectId() const { return m_ObjectID; }

		template<typename Component_t, typename... Args>
		Component_t& AddComponent(Args... args)
		{
			return m_world.Registry().emplace<Component_t>(static_cast<entt::entity>(m_entityID), std::forward<Args>(args)...);
		}

		template<typename Component_t>
		Component_t& GetComponent()
		{
			return m_world.Registry().get<Component_t>(static_cast<entt::entity>(m_entityID));
		}
		template<typename Component_t>
		Component_t GetComponent() const
		{
			return m_world.Registry().get<Component_t>(static_cast<entt::entity>(m_entityID));
		}


		components::Position& Position() { return GetComponent<components::Position>(); }
		//components::Position Position() const { return GetComponent<components::Position>(); }

		components::Rotation& Rotation() { return GetComponent<components::Rotation>(); }
		//components::Rotation Rotation() const { return GetComponent<components::Rotation>(); }

		components::Scale& Scale() { return GetComponent<components::Scale>(); }
		//components::Scale Scale() const { return GetComponent<components::Scale>(); }

		components::BoundingVolume& BoundingVolume() { return GetComponent<components::BoundingVolume>(); }

	private:
		World& m_world;
		uint32_t m_entityID;

		static uint64_t ms_lastObjectID;
		uint64_t m_ObjectID;

		std::string m_name;
		/*components::Position* m_pos;
		components::Rotation* m_rot;
		components::Scale* m_scale;*/
		//std::shared_ptr<EntityState> m_state;
	};
}