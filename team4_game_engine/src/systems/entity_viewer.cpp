#include <team4_game_engine/systems/entity_viewer.hpp>
#include <spdlog/spdlog.h>
#include <team4_game_engine/components/name.hpp>
#include <team4_game_engine/components/position.hpp>

namespace team4_game_engine::systems {
	class EntityViewerImpl {
	public:
		EntityViewerImpl() {

		}

		virtual ~EntityViewerImpl() {
		}
		EntityViewerImpl(const  EntityViewerImpl& other) = delete;
		EntityViewerImpl(EntityViewerImpl&& other) = delete;
		void Update(std::chrono::milliseconds deltatime, engine::World& world) {
			m_totalMilli += deltatime;
			if (m_totalMilli < 1s) return;
			world.Registry().view<components::Name, components::Position>().each([](components::Name& name, components::Position& pos) {
				spdlog::info("Entity {} is at X:{}/Y:{}/Z:{}", name, pos.local.x, pos.local.y, pos.local.z);
			});
			m_totalMilli = 0ms;
			/*if (m_totalMilli >= 1s) {
				world.Registry().view<components::Name, components::Position>().each([](components::Name& name, components::Position& pos) {
					spdlog::info("Entity {} is at X:{}/Y:{}/Z:{}", name, pos.x, pos.y, pos.z);
				});
				m_totalMilli = 0ms;
			}*/
		}
	private:
		std::chrono::milliseconds m_totalMilli = 0ms;
	};

	EntityViewer::EntityViewer() {
		m_impl = std::make_unique<EntityViewerImpl>();
	}

	EntityViewer::~EntityViewer() {
		m_impl = nullptr;
	}

	void EntityViewer::Update(std::chrono::milliseconds deltatime, engine::World& world) {
		m_impl->Update(deltatime, world);
	}
}