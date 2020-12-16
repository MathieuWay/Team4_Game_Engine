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
		const int maxEntities = 1;
		const int MaxDepth = 3;

		
		bool isDivided;
		int depth;

		entt::registry& registry = engine::Engine::Instance().GetWorld().lock()->Registry();
		Vector3D position;
		Vector3D size;
		OctTree* child[8];
		vector<entt::entity> entities;
		
		// cree un octTree d'une taille size a une certaine position(correspond au point centrale) et a une profondeur dans l'octTree 
		OctTree(Vector3D position, Vector3D size, int depth);

		//Verifie si l'entity est en colission avec les Octree enfants a cote de l'Octree numero index
		vector<int> IntersectWith(entt::entity entity, int index, vector<int> toInsert);
			
		unsigned int getChildrenIndex(const Vector3D& objectCenter);
		
		// ajoute une entity à l'octree et l'envoie dans le bonne enfant si il y en a 
		void addEntity(entt::entity entity);

		// divise l'octree en 8 nouveaux octree enfants
		void divide();

		// recupere des couples de collision a tester grace à l'Octree
		vector<collisionCouple> query(vector<collisionCouple>& resRegister);
	};

}