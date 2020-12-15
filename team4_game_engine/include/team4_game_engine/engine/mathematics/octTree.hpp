#pragma once
#include <team4_game_engine/engine/entity.hpp>
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
#include <team4_game_engine/engine/engine.hpp>
#include <team4_game_engine\components\components.hpp>

using namespace team4_game_engine::components;
using namespace team4_game_engine::engine::mathematics;

namespace team4_game_engine::engine::mathematics {

	
	struct collisionCouple {
		entt::entity ent1;
		entt::entity ent2;
	}; 
	
	class OctTree;

	class OctTree {
	public:
		const int maxEntities = 2;
		const int MaxDepth = 3;

		
		bool isDivided;
		int depth;

		entt::registry& registry = engine::Engine::Instance().GetWorld().lock()->Registry();
		Vector3D position;
		Vector3D size;
		OctTree* child[8];
		vector<entt::entity> entities;
		
		OctTree(Vector3D position, Vector3D size, int depth);

		vector<int> IntersectWith(entt::entity entity, int index, vector<int> toInsert);
			
		unsigned int getChildrenIndex(const Vector3D& objectCenter);

		void addEntity(entt::entity entity);

		void divide();

		vector<collisionCouple> query(vector<collisionCouple>& resRegister);
	};

}