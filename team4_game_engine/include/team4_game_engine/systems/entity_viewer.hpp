#pragma once
#include <team4_game_engine/common.hpp>
#include <team4_game_engine/systems/system.hpp>

namespace team4_game_engine::systems {
	class EntityViewerImpl;
	class EntityViewer : public ISystem {
	public:
		EntityViewer();
		virtual ~EntityViewer();

		EntityViewer(const  EntityViewer& other) = delete;
		EntityViewer(EntityViewer&& other) = delete;

		virtual void Update(std::chrono::milliseconds deltatime, engine::World& world) override;
	private:
		std::unique_ptr<EntityViewerImpl> m_impl;
	};
}