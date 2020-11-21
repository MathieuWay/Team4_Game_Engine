#pragma once


#include <team4_game_engine/common.hpp>

#include <team4_game_engine/components/name.hpp>
#include <entt/entt.hpp>

namespace team4_game_engine::engine
{
	class Entity;
	class WorldImpl;
	class World
	{
	public:
		explicit World(const std::string& name);
		~World();

		World(const World& other) = delete;
		World(World&& other) = delete;

		template<typename Component_t, typename... Args>
		Component_t& AddComponent(entt::entity entity,Args... args)
		{
			return Registry().emplace<Component_t>(entity, std::forward<Args>(args)...);
		}
		template<typename Component_t>
		Component_t& GetComponent(entt::entity entity)
		{
			return Registry().get<Component_t>(entity);
		}

		entt::registry& Registry();
		uint32_t CreateEntity();
		void Clear();
	private:
		WorldImpl* m_impl;
	};
}