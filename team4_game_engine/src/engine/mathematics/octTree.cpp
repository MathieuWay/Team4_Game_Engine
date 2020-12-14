#include <team4_game_engine/engine/mathematics/octTree.hpp>
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
#include <team4_game_engine/engine/entity.hpp>

namespace team4_game_engine::engine::mathematics {

	OctTree::OctTree(Vector3D position, Vector3D size, int depth) {
		this->isDivided = false;
		this->position = position;
		this->depth = depth;
		this->size = size;
	}

	bool OctTree::IntersectWith(entt::entity entity)
	{
		Vector3D center = registry.get<Position>(entity).local;
		float radius = registry.get<BoundingVolume>(entity).radius;

		const float hw = this->size.x / 2;
		const float hh = this->size.y / 2;
		const float hd = this->size.z / 2;

		// face 1
		Vector3D A = this->position.sumVector(Vector3D(hw, 0, 0));
		Vector3D B = this->position.sumVector(Vector3D(hw, hh, 0));
		Vector3D C = this->position.sumVector(Vector3D(hw, 0, hd));

		Vector3D AB = A.subVector(B);
		Vector3D AC = A.subVector(C);

		Vector3D normal = AB.vectorProduct(AC);
		
		float d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z - normal.z) / normal.magnitude();
		if (d < radius) {
			return true;
		 }

		// face 2
		 A = this->position.sumVector(Vector3D(-hw, 0, 0));
		 B = this->position.sumVector(Vector3D(-hw, hh, 0));
		 C = this->position.sumVector(Vector3D(-hw, 0, hd));

		 AB = A.subVector(B);
		 AC = A.subVector(C);

		 normal = AB.vectorProduct(AC);

		 d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z - normal.z) / normal.magnitude();
		if (d < radius) {
			return true;
		}

		// face 3
		 A = this->position.sumVector(Vector3D(0, hh, 0));
		 B = this->position.sumVector(Vector3D(hw, hh, 0));
		 C = this->position.sumVector(Vector3D(0, hh, hd));

		 AB = A.subVector(B);
		 AC = A.subVector(C);

		 normal = AB.vectorProduct(AC);

		 d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z - normal.z) / normal.magnitude();
		if (d < radius) {
			return true;
		}

		// face 4
		 A = this->position.sumVector(Vector3D(0, -hh, 0));
		 B = this->position.sumVector(Vector3D(hw, -hh, 0));
		 C = this->position.sumVector(Vector3D(0, -hh, hd));

		 AB = A.subVector(B);
		 AC = A.subVector(C);

		 normal = AB.vectorProduct(AC);

		 d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z - normal.z) / normal.magnitude();
		if (d < radius) {
			return true;
		}

		// face 5
		 A = this->position.sumVector(Vector3D(0, 0, hd));
		 B = this->position.sumVector(Vector3D(0, hh, hd));
		 C = this->position.sumVector(Vector3D(hw, 0, hd));

		 AB = A.subVector(B);
		 AC = A.subVector(C);

		 normal = AB.vectorProduct(AC);

		 d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z - normal.z) / normal.magnitude();
		if (d < radius) {
			return true;
		}

		// face 6
		 A = this->position.sumVector(Vector3D(0, 0, -hd));
		 B = this->position.sumVector(Vector3D(0, hh, -hd));
		 C = this->position.sumVector(Vector3D(hw, 0, -hd));

		 AB = A.subVector(B);
		 AC = A.subVector(C);

		 normal = AB.vectorProduct(AC);

		 d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z - normal.z) / normal.magnitude();
		if (d < radius) {
			return true;
		}
		return false;
	}

	unsigned int OctTree::getChildrenIndex(const Vector3D& objectCenter) {
		unsigned int index = 0;
		if (objectCenter.x > position.x) index += 2;
		if (objectCenter.y < position.y) index += 1;
		if (objectCenter.z < position.z) index += 4;
		return index;
	}

	void OctTree::addEntity(entt::entity entity) {
		if (this->depth == this->MaxDepth || (!this->isDivided && this->entities.size() < this->maxEntities)) {
			this->entities.push_back(entity);
		}
		else {
			if (!this->isDivided) {
				this->divide();
			}
			bool hasIntersect = false;
			for (int i = 0; i < 8; i++) {
				if (this->child[i]->IntersectWith(entity)) {
					this->child[i]->addEntity(entity);
					hasIntersect = true;
				}
			}
			if (!hasIntersect) {
				unsigned int index = this->getChildrenIndex(registry.get<Position>(entity).local);
				this->child[index]->addEntity(entity);
			}
		}
	};

	void OctTree::divide() {
		const float hw = this->size.x / 2;
		const float hh = this->size.y / 2;
		const float hd = this->size.z / 2;
		Vector3D newSize = Vector3D(hw, hh, hd);
		this->child[0] = new OctTree(Vector3D(this->position.x - hw / 2, this->position.y - hh / 2, this->position.z + hd / 2), newSize, this->depth + 1);
		this->child[1] = new OctTree(Vector3D(this->position.x - hw / 2, this->position.y + hh / 2, this->position.z + hd / 2), newSize, this->depth + 1);
		this->child[2] = new OctTree(Vector3D(this->position.x + hw / 2, this->position.y - hh / 2, this->position.z + hd / 2), newSize, this->depth + 1);
		this->child[3] = new OctTree(Vector3D(this->position.x + hw / 2, this->position.y + hh / 2, this->position.z + hd / 2), newSize, this->depth + 1);
		this->child[4] = new OctTree(Vector3D(this->position.x - hw / 2, this->position.y - hh / 2, this->position.z - hd / 2), newSize, this->depth + 1);
		this->child[5] = new OctTree(Vector3D(this->position.x - hw / 2, this->position.y + hh / 2, this->position.z - hd / 2), newSize, this->depth + 1);
		this->child[6] = new OctTree(Vector3D(this->position.x + hw / 2, this->position.y - hh / 2, this->position.z - hd / 2), newSize, this->depth + 1);
		this->child[7] = new OctTree(Vector3D(this->position.x + hw / 2, this->position.y + hh / 2, this->position.z - hd / 2), newSize, this->depth + 1);

		for (int i = 0; i < this->entities.size(); i++) {
			entt::entity entity = entities[i];
			
			bool hasIntersect = false;
			for (int i = 0; i < 8; i++) {
				if (this->child[i]->IntersectWith(entity)) {
					this->child[i]->addEntity(entity);
					hasIntersect = true;
				}
			}
			if (!hasIntersect) {
				unsigned int index = this->getChildrenIndex(registry.get<Position>(entity).local);
				this->child[index]->addEntity(entity);
			}
		}

		this->entities.clear();
		this->isDivided = true;
	}
	vector<collisionCouple> OctTree::query(vector<collisionCouple>& resRegister) {
		if (!isDivided) {
			if (this->entities.size() > 1) {
				for (int i = 0; i < this->entities.size(); i++) {
					for (int j = i + 1; j < this->entities.size(); j++) {
						collisionCouple couple;
						couple.ent1 = this->entities[i];
						couple.ent2 = this->entities[j];
						bool hasCouple = false;
						for (int k = 0; k < resRegister.size(); k++) {
							if ((resRegister[k].ent1 == couple.ent1 && resRegister[k].ent2 == couple.ent2) || (resRegister[k].ent1 == couple.ent2 && resRegister[k].ent2 == couple.ent1)) {
								hasCouple = true;
							}
						}
						if (!hasCouple) {
							resRegister.push_back(couple);
						}
					}
				}
			}
			return resRegister;
		}
		else {
			this->child[0]->query(resRegister);
			this->child[1]->query(resRegister);
			this->child[2]->query(resRegister);
			this->child[3]->query(resRegister);
			this->child[4]->query(resRegister);
			this->child[5]->query(resRegister);
			this->child[6]->query(resRegister);
			this->child[7]->query(resRegister);
			return resRegister;
		}
	}
/*
query (area: Rectangle, out: ColliderComponent[][] = []): ColliderComponent[][] {
	  if (!this.bounds.intersectsWith(area)) {
		return out
	  } else {
		if (!this.divided) {
		  out.push([]);
		  for (const component of this.components) {
			if (area.intersectsWith(component.area)) out[out.length - 1].push(component)
		  }

		} else {
		  this.northWest.query(area, out)
		  this.northEast.query(area, out)
		  this.southWest.query(area, out)
		  this.southEast.query(area, out)
		}

		return out
	  }
	}

*/

}