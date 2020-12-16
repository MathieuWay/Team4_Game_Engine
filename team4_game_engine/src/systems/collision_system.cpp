#include <team4_game_engine/common.hpp>
#include <spdlog/spdlog.h>

#include <team4_game_engine/systems/collision_system.hpp>
#include <team4_game_engine/engine/physics/physics.hpp>
#include <team4_game_engine/engine/physics/collision.hpp>
#include <team4_game_engine/engine/physics/collision_resolver.hpp>
#include <team4_game_engine/engine/physics/collision_registry.hpp>
#include <team4_game_engine/engine/physics/CollisionData.hpp>
#include <team4_game_engine/engine/physics/Contact.hpp>
using namespace team4_game_engine::physics;

#include <team4_game_engine/engine/engine.hpp>
#include <team4_game_engine/engine/world.hpp>

#include <team4_game_engine/engine/mathematics/vector3d.hpp>
using namespace team4_game_engine::engine::mathematics;

#include <team4_game_engine/components/position.hpp>
#include <team4_game_engine/components/scale.hpp>
#include <team4_game_engine/components/rigidbody.hpp>
#include <team4_game_engine/components/colliders/plane_collider.hpp>
#include <team4_game_engine/components/colliders/box_collider.hpp>
#include <team4_game_engine/components/colliders/sphere_collider.hpp>
#include <team4_game_engine\engine\mathematics\octTree.hpp>
using namespace team4_game_engine::components;

namespace team4_game_engine::systems {
	struct OBB {
		Vector3D vertex[8];
		OBB(RigidBody rb) {
			BoxData aBox = ((BoxCollider*)rb.collider)->GetBoxData();
			vertex[0] = Vector3D(aBox.min.x, aBox.min.y, aBox.min.z);
			vertex[1] = Vector3D(aBox.min.x, aBox.min.y, aBox.max.z);
			vertex[2] = Vector3D(aBox.min.x, aBox.max.y, aBox.min.z);
			vertex[3] = Vector3D(aBox.min.x, aBox.max.y, aBox.max.z);
			vertex[4] = Vector3D(aBox.max.x, aBox.min.y, aBox.min.z);
			vertex[5] = Vector3D(aBox.max.x, aBox.min.y, aBox.max.z);
			vertex[6] = Vector3D(aBox.max.x, aBox.max.y, aBox.min.z);
			vertex[7] = Vector3D(aBox.max.x, aBox.max.y, aBox.max.z);
			for (int i = 0; i < 8; i++) vertex[i] = Matrix4().localToWorld(vertex[i], rb.transforMatrix);
		}
	};
	class CollisionSystemImpl {
	public:
		CollisionSystemImpl(int resolveIteration) : colRegistry(resolveIteration){
		}

		// Boucle physique
		void Update(std::chrono::milliseconds deltatime, engine::World& world) {
			// Si la simulation est en pause ou que la prochaine frame n'a pas été demandé on n'execute pas le reste de la fonction
			if (!Physics::doPhysicsStep && !Physics::doNextStep) return;
			// Si les collisions de la simulation ne sont pas activé on n'execute pas le reste de la fonction
			if (!Physics::doCollisionStep) return;

			// Si la simulation utilise le un delta fixe alors on vérifie que le délai a bien été effectué
			if (Physics::useFixedDeltatime) {
				m_totalMilli += deltatime;
				if (m_totalMilli < 1s) return;
				m_totalMilli = 0ms;
			}

			// Calcule du deltatime
			float delta = Physics::useFixedDeltatime ? Physics::physicsStepTime : (float)deltatime.count() / (float)1000;

			// Octree
			OctTree octTree = OctTree(Vector3D(0,0,0),Vector3D(25,25,25),0);
			auto view = world.Registry().view<Position, Rotation, Scale, RigidBody, BoundingVolume>();
			// insere chaque entité dans l'octree
			for (auto it = view.begin(); it != view.end(); it++) {
				octTree.addEntity(*it);
			}
			// Recupere la liste des collisions possible provenant de l'octree
			std::vector<collisionCouple> collisions = octTree.query(std::vector<collisionCouple>());

			// boucle sur les collisions
			for (int i = 0; i < collisions.size(); i++)
			{
				RigidBody& rbA = view.get<RigidBody>(collisions[i].ent1);
				RigidBody& rbB = view.get<RigidBody>(collisions[i].ent2);
				CollisionData* collisionData = nullptr;

				// Si collision entre Box & Plane
				if ((rbA.collider->GetShape() == Shape::Box && rbB.collider->GetShape() == Shape::Plane)) {
					collisionData = IsBoxToPlaneColliding(collisions[i].ent1, collisions[i].ent2);
				}
				else if (rbA.collider->GetShape() == Shape::Plane && rbB.collider->GetShape() == Shape::Box) {
					collisionData = IsBoxToPlaneColliding(collisions[i].ent2, collisions[i].ent1);
				}
				// Si collision entre Box & Box
				//if ((rbA.collider->GetShape() == Shape::Box && rbB.collider->GetShape() == Shape::Box)) {
				//	collisionData = IsBoxToBoxColliding(collisions[i].ent1, collisions[i].ent2);
				//}

				// Si il y a belle et bien une collision on l'ajoute au registre
				if (collisionData != nullptr) colRegistry.AddCollision(collisionData);

			}

			// Si il y a au moins une collision on stop l'éxécution de notre boucle physique
			if (colRegistry.size() > 0 && Physics::doPhysicsStep) Physics::doPhysicsStep = false;

			// résoudre les collisions
			colRegistry.Resolve();
		}

		// calcule la distance entre un point et un plan par rapport a sa normale
		float DistanceWithPlane(Vector3D point, Vector3D normal, Vector3D planePosition) {
			float offset = -normal.scalarProduct(planePosition);
			float scalar = normal.scalarProduct(point);
			float result = scalar + offset;
			return result;
		}

		// Vérifie si il y a bien une collision entre un cube et un plan
		CollisionData* IsBoxToPlaneColliding(entt::entity cube, entt::entity plane) {
			RigidBody& rbCube = engine::Engine::Instance().GetWorld().lock()->Registry().get<RigidBody>(cube);
			RigidBody& rbPlane = engine::Engine::Instance().GetWorld().lock()->Registry().get<RigidBody>(plane);
			Position& posPlane = engine::Engine::Instance().GetWorld().lock()->Registry().get<Position>(plane);
			OBB aOBB = OBB(rbCube);
			PlaneData bPlane = ((PlaneCollider*)rbPlane.collider)->GetPlaneData();
			int i = 0;
			float maxDistance = 0;
			std::vector<Contact*> verticesIndex = std::vector<Contact*>();
			float maxVertexIndex = 0;
			// boucle sur les 8 sommet du cube
			while (i < 8) {
				float distance = DistanceWithPlane(aOBB.vertex[i], Matrix4().localToWorldDirn(bPlane.normal, rbPlane.transforMatrix), posPlane.local);
				// Si le sommet est derrière le plan par rapport a la direction de la normale
				if(distance < 0) {
					verticesIndex.push_back(new Contact(
						aOBB.vertex[i],
						Matrix4().localToWorldDirn(bPlane.normal, rbPlane.transforMatrix),
						distance
					));
				}
				i++;
			}
			// Si il y a au moins une collision avec un sommet alors on retourne les données de la collision
			if (verticesIndex.size()) {
				return new CollisionData(verticesIndex, cube, plane);
			}
			return nullptr;
		}
	private:
		std::chrono::milliseconds m_totalMilli = 0ms;
		CollisionRegistry colRegistry;

	};

	CollisionSystem::CollisionSystem(int resolveIteration) {
		m_impl = std::make_unique<CollisionSystemImpl>(resolveIteration);
	}

	CollisionSystem::~CollisionSystem() {
		m_impl = nullptr;
	}

	void CollisionSystem::Update(std::chrono::milliseconds deltatime, engine::World& world) {
		m_impl->Update(deltatime, world);
	}
}