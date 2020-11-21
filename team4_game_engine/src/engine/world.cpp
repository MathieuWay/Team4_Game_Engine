#include <team4_game_engine/engine/world.hpp>
#include <team4_game_engine/components/model.hpp>
#include <team4_game_engine/engine/renderer/renderer3d.hpp>

namespace team4_game_engine::engine
{
	class WorldImpl {
	public:
		explicit WorldImpl(const std::string& name)
		{

		}

		~WorldImpl()
		{
			m_registry.clear();
		}

		WorldImpl(const WorldImpl& other) = delete;
		WorldImpl(WorldImpl&& other) = delete;

		entt::registry& Registry()
		{
			return m_registry;
		}

		uint32_t CreateEntity() {
			return static_cast<uint32_t>(m_registry.create());
		}

		void Clear() {
			m_registry.clear();
			for (auto& mesh : renderer::Renderer3D::Instance().ms_meshs) {
				mesh->instanceReferences.clear();
			}
		}

	private:
		entt::registry m_registry;
	};


	World::World(const std::string& name) {
		m_impl = new WorldImpl(name);
	}
	World::~World() {
		delete m_impl;
	}

	entt::registry& World::Registry() {
		return m_impl->Registry();
	}
	uint32_t World::CreateEntity() {
		return m_impl->CreateEntity();
	}
	void World::Clear() {
		m_impl->Clear();
	}
}