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

	//vérifie si l'entity est en collision avec les cotes de l'octTree numero "index" 
	vector<int> OctTree::IntersectWith(entt::entity entity, int index,vector<int> toInsert)
	{
		Vector3D center = registry.get<Position>(entity).local;
		float radius = registry.get<BoundingVolume>(entity).radius;

		const float hw = this->size.x / 2;
		const float hh = this->size.y / 2;
		const float hd = this->size.z / 2;

		bool face1 = false;
		bool face2 = false;
		bool face3 = false;
		
		Vector3D A;
		Vector3D B;
		Vector3D C;
		Vector3D AB;
		Vector3D AC;
		Vector3D normal;
		float d;
		float Pland;
		switch (index)
		{
		case 0:
			// face 1
			 A = this->position.sumVector(Vector3D(hw, 0, 0));
			 B = this->position.sumVector(Vector3D(hw, hh, 0));
			 C = this->position.sumVector(Vector3D(hw, 0, hd));

			 AB = A.subVector(B);
			 AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);
			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(2);
				face1 = true;
			}
			//face 2
			A = this->position.sumVector(Vector3D(0, -hh, 0));
			B = this->position.sumVector(Vector3D(hw, -hh, 0));
			C = this->position.sumVector(Vector3D(0, -hh, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(1);
				face2 = true;
			}
			// face 3
			A = this->position.sumVector(Vector3D(0, 0, -hd));
			B = this->position.sumVector(Vector3D(0, hh, -hd));
			C = this->position.sumVector(Vector3D(hw, 0, -hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(4);
				face3 = true;
			}

			if (face1 && face2) {
				toInsert.push_back(3);
			}
			if (face1 && face3) {
				toInsert.push_back(6);
			}
			if (face2 && face3) {
				toInsert.push_back(5);
			}
			if (face1 && face2 && face3) {
				toInsert.push_back(7);
			}
			break;
		case 1:
			//face 1
			 A = this->position.sumVector(Vector3D(hw, 0, 0));
			 B = this->position.sumVector(Vector3D(hw, hh, 0));
			 C = this->position.sumVector(Vector3D(hw, 0, hd));

			 AB = A.subVector(B);
			 AC = A.subVector(C);

			 normal = AB.vectorProduct(AC);
			 Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(3);
				face1 = true;
			}
			// face 2
			A = this->position.sumVector(Vector3D(0, hh, 0));
			B = this->position.sumVector(Vector3D(hw, hh, 0));
			C = this->position.sumVector(Vector3D(0, hh, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(0);
				face2 = true;
			}
			
			// face 3
			A = this->position.sumVector(Vector3D(0, 0, -hd));
			B = this->position.sumVector(Vector3D(0, hh, -hd));
			C = this->position.sumVector(Vector3D(hw, 0, -hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(5);
				face3 = true;
			}

			if (face1 && face2) {
				toInsert.push_back(2);
			}
			if (face1 && face3) {
				toInsert.push_back(7);
			}
			if (face2 && face3) {
				toInsert.push_back(4);
			}
			if (face1 && face2 && face3) {
				toInsert.push_back(6);
			}
			break;
		case 2:

			// face 1
			A = this->position.sumVector(Vector3D(-hw, 0, 0));
			B = this->position.sumVector(Vector3D(-hw, hh, 0));
			C = this->position.sumVector(Vector3D(-hw, 0, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(0);
				face1 = true;
			}

			//face 2
			A = this->position.sumVector(Vector3D(0, -hh, 0));
			B = this->position.sumVector(Vector3D(hw, -hh, 0));
			C = this->position.sumVector(Vector3D(0, -hh, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(3);
				face2 = true;
			}
			// face 3
			A = this->position.sumVector(Vector3D(0, 0, -hd));
			B = this->position.sumVector(Vector3D(0, hh, -hd));
			C = this->position.sumVector(Vector3D(hw, 0, -hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(6);
				face3 = true;
			}
			if (face1 && face2) {
				toInsert.push_back(1);
			}
			if (face1 && face3) {
				toInsert.push_back(4);
			}
			if (face2 && face3) {
				toInsert.push_back(7);
			}
			if (face1 && face2 && face3) {
				toInsert.push_back(5);
			}
			break;
		case 3:
			// face 1
			A = this->position.sumVector(Vector3D(-hw, 0, 0));
			B = this->position.sumVector(Vector3D(-hw, hh, 0));
			C = this->position.sumVector(Vector3D(-hw, 0, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(1);
				face1 = true;
			}
			// face 2
			A = this->position.sumVector(Vector3D(0, hh, 0));
			B = this->position.sumVector(Vector3D(hw, hh, 0));
			C = this->position.sumVector(Vector3D(0, hh, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(2);
				face2 = true;
			}
			// face 3
			A = this->position.sumVector(Vector3D(0, 0, -hd));
			B = this->position.sumVector(Vector3D(0, hh, -hd));
			C = this->position.sumVector(Vector3D(hw, 0, -hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(7);
				face3 = true;
			}

			if (face1 && face2) {
				toInsert.push_back(0);
			}
			if (face1 && face3) {
				toInsert.push_back(5);
			}
			if (face2 && face3) {
				toInsert.push_back(6);
			}
			if (face1 && face2 && face3) {
				toInsert.push_back(4);
			}
			break;
		case 4:
			//face 1
			A = this->position.sumVector(Vector3D(hw, 0, 0));
			B = this->position.sumVector(Vector3D(hw, hh, 0));
			C = this->position.sumVector(Vector3D(hw, 0, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(6);
				face1 = true;
			}
			//face 2
			A = this->position.sumVector(Vector3D(0, -hh, 0));
			B = this->position.sumVector(Vector3D(hw, -hh, 0));
			C = this->position.sumVector(Vector3D(0, -hh, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(5);
				face2 = true;
			}
			// face 3
			A = this->position.sumVector(Vector3D(0, 0, hd));
			B = this->position.sumVector(Vector3D(0, hh, hd));
			C = this->position.sumVector(Vector3D(hw, 0, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(0);
				face3 = true;
			}

			if (face1 && face2) {
				toInsert.push_back(7);
			}
			if (face1 && face3) {
				toInsert.push_back(2);
			}
			if (face2 && face3) {
				toInsert.push_back(1);
			}
			if (face1 && face2 && face3) {
				toInsert.push_back(3);
			}
			break;
		case 5:
			//face 1
			 A = this->position.sumVector(Vector3D(hw, 0, 0));
			 B = this->position.sumVector(Vector3D(hw, hh, 0));
			 C = this->position.sumVector(Vector3D(hw, 0, hd));

			 AB = A.subVector(B);
			 AC = A.subVector(C);

			 normal = AB.vectorProduct(AC);
			 Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			 d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(7);
				face1 = true;
			}
			// face 2
			A = this->position.sumVector(Vector3D(0, hh, 0));
			B = this->position.sumVector(Vector3D(hw, hh, 0));
			C = this->position.sumVector(Vector3D(0, hh, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(4);
				face2 = true;
			}
			// face 3
			A = this->position.sumVector(Vector3D(0, 0, hd));
			B = this->position.sumVector(Vector3D(0, hh, hd));
			C = this->position.sumVector(Vector3D(hw, 0, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(1);
				face3 = true;
			}

			if (face1 && face2) {
				toInsert.push_back(6);
			}
			if (face1 && face3) {
				toInsert.push_back(3);
			}
			if (face2 && face3) {
				toInsert.push_back(0);
			}
			if (face1 && face2 && face3) {
				toInsert.push_back(2);
			}
			break;
		case 6:
			// face 1
			A = this->position.sumVector(Vector3D(-hw, 0, 0));
			B = this->position.sumVector(Vector3D(-hw, hh, 0));
			C = this->position.sumVector(Vector3D(-hw, 0, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(4);
				face1 = true;
			}
			//face 2
			A = this->position.sumVector(Vector3D(0, -hh, 0));
			B = this->position.sumVector(Vector3D(hw, -hh, 0));
			C = this->position.sumVector(Vector3D(0, -hh, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(7);
				face2 = true;
			}
			// face 3
			A = this->position.sumVector(Vector3D(0, 0, hd));
			B = this->position.sumVector(Vector3D(0, hh, hd));
			C = this->position.sumVector(Vector3D(hw, 0, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(2);
				face3 = true;
			}

			if (face1 && face2) {
				toInsert.push_back(5);
			}
			if (face1 && face3) {
				toInsert.push_back(0);
			}
			if (face2 && face3) {
				toInsert.push_back(3);
			}
			if (face1 && face2 && face3) {
				toInsert.push_back(1);
			}

			break;
		case 7:
			// face 1
			A = this->position.sumVector(Vector3D(-hw, 0, 0));
			B = this->position.sumVector(Vector3D(-hw, hh, 0));
			C = this->position.sumVector(Vector3D(-hw, 0, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(5);
				face1 = true;
			}
			// face 2
			A = this->position.sumVector(Vector3D(0, hh, 0));
			B = this->position.sumVector(Vector3D(hw, hh, 0));
			C = this->position.sumVector(Vector3D(0, hh, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(6);
				face2 = true;
			}
			// face 3
			A = this->position.sumVector(Vector3D(0, 0, hd));
			B = this->position.sumVector(Vector3D(0, hh, hd));
			C = this->position.sumVector(Vector3D(hw, 0, hd));

			AB = A.subVector(B);
			AC = A.subVector(C);

			normal = AB.vectorProduct(AC);
			Pland = -(A.x * normal.x + A.y * normal.y + A.z * normal.z);

			d = fabsf(normal.x * center.x + normal.y * center.y + normal.z * center.z + Pland) / normal.magnitude();
			if (d < radius) {
				toInsert.push_back(3);
				face3 = true;
			}

			if (face1 && face2) {
				toInsert.push_back(4);
			}
			if (face1 && face3) {
				toInsert.push_back(1);
			}
			if (face2 && face3) {
				toInsert.push_back(2);
			}
			if (face1 && face2 && face3) {
				toInsert.push_back(0);
			}

			break;
		}
		
		return toInsert;
	}

	// determine dans quelle enfant de l'octree on doit envoyé l'entity
	unsigned int OctTree::getChildrenIndex(const Vector3D& objectCenter) {
		unsigned int index = 0;
		if (objectCenter.x > position.x) index += 2;
		if (objectCenter.y < position.y) index += 1;
		if (objectCenter.z < position.z) index += 4;
		return index;
	}
	// ajoute une entity
	void OctTree::addEntity(entt::entity entity) {
		if (this->depth == this->MaxDepth || (!this->isDivided && this->entities.size() < this->maxEntities)) {
			this->entities.push_back(entity);
		}
		else {
			if (!this->isDivided) {
				this->divide();
			}
			unsigned int index = this->getChildrenIndex(registry.get<Position>(entity).local);
			vector<int> toInsert = this->child[index]->IntersectWith(entity, index, {});
			for (int i = 0; i < toInsert.size(); i++) {
				this->child[toInsert[i]]->addEntity(entity);
			}
			this->child[index]->addEntity(entity);
		}
	};
	// divise l'octree
	void OctTree::divide() {
		const float hw = this->size.x / 2;
		const float hh = this->size.y / 2;
		const float hd = this->size.z / 2;
		Vector3D newSize = Vector3D(hw, hh, hd);
		this->child[0] = new OctTree(Vector3D(this->position.x - hw / 2, this->position.y + hh / 2, this->position.z + hd / 2), newSize, this->depth + 1);
		this->child[1] = new OctTree(Vector3D(this->position.x - hw / 2, this->position.y - hh / 2, this->position.z + hd / 2), newSize, this->depth + 1);
		this->child[2] = new OctTree(Vector3D(this->position.x + hw / 2, this->position.y + hh / 2, this->position.z + hd / 2), newSize, this->depth + 1);
		this->child[3] = new OctTree(Vector3D(this->position.x + hw / 2, this->position.y - hh / 2, this->position.z + hd / 2), newSize, this->depth + 1);
		this->child[4] = new OctTree(Vector3D(this->position.x - hw / 2, this->position.y + hh / 2, this->position.z - hd / 2), newSize, this->depth + 1);
		this->child[5] = new OctTree(Vector3D(this->position.x - hw / 2, this->position.y - hh / 2, this->position.z - hd / 2), newSize, this->depth + 1);
		this->child[6] = new OctTree(Vector3D(this->position.x + hw / 2, this->position.y + hh / 2, this->position.z - hd / 2), newSize, this->depth + 1);
		this->child[7] = new OctTree(Vector3D(this->position.x + hw / 2, this->position.y - hh / 2, this->position.z - hd / 2), newSize, this->depth + 1);

		for (int i = 0; i < this->entities.size(); i++) {
			entt::entity entity = entities[i];
			unsigned int index = this->getChildrenIndex(registry.get<Position>(entity).local);
			vector<int> toInsert = this->child[index]->IntersectWith(entity, index, {});
			for (int i = 0; i < toInsert.size(); i++) {
				this->child[toInsert[i]]->addEntity(entity);
			}
			this->child[index]->addEntity(entity);
		}

		this->entities.clear();
		this->isDivided = true;
	}
	// recupere des couples de collision grace à l'Octree
	vector<collisionCouple> OctTree::query(vector<collisionCouple>& resRegister) {
		if (!isDivided) {
			if (this->entities.size() > 1) {
				for (int i = 0; i < this->entities.size(); i++) {
					for (int j = i + 1; j < this->entities.size(); j++) {
						collisionCouple couple;
						couple.ent1 = this->entities[i];
						couple.ent2 = this->entities[j];
						if (couple.ent1 != couple.ent2) {
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