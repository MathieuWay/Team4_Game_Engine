#pragma once
#include <team4_game_engine/common.hpp>
#include <team4_game_engine/engine/physics/CollisionData.hpp>

namespace team4_game_engine::physics {
	class CollisionRegistry {
	public:
		CollisionRegistry(int limit) : resolveLimit(limit){

		}

		void AddCollision(CollisionData* collisionData) {
			if (collisionList.size() >= resolveLimit) return;

			collisionList.push_back(collisionData);
		}

		void Resolve() {
			for (int i = 0; i < collisionList.size(); i++)
			{
				collisionList[i]->ResolveContact();
				delete collisionList[i];
			}
			collisionList.clear();
		}

		size_t size() {
			return collisionList.size();
		}
	private:
		int resolveLimit;
		std::vector<CollisionData*> collisionList;
	};
}