#include <team4_game_engine/common.hpp>
#include <spdlog/spdlog.h>

#include <team4_game_engine/systems/collision_system.hpp>
#include <team4_game_engine/engine/physics/physics.hpp>
#include <team4_game_engine/engine/physics/collision.hpp>
#include <team4_game_engine/engine/physics/collision_resolver.hpp>
using namespace team4_game_engine::physics;

#include <team4_game_engine/engine/engine.hpp>
#include <team4_game_engine/engine/world.hpp>

#include <team4_game_engine/engine/mathematics/vector3d.hpp>
using namespace team4_game_engine::engine::mathematics;

#include <team4_game_engine/components/position.hpp>
#include <team4_game_engine/components/scale.hpp>
#include <team4_game_engine/components/rigidbody.hpp>
#include <team4_game_engine/components/colliders/box_collider.hpp>
#include <team4_game_engine/components/colliders/sphere_collider.hpp>
using namespace team4_game_engine::components;

namespace team4_game_engine::systems {
	struct State{
		entt::entity entity;
		Position& pos;
		Scale& scale;
		RigidBody* rb;
	};
	class CollisionSystemImpl {
	public:
		CollisionSystemImpl(int resolveIteration) : resolver(resolveIteration){
		}
		void Update(std::chrono::milliseconds deltatime, engine::World& world) {
			if (!Physics::doPhysicsStep && !Physics::doNextStep) return;
			if (Physics::useFixedDeltatime) {
				m_totalMilli += deltatime;
				if (m_totalMilli < 1s) return;
				m_totalMilli = 0ms;
			}
			float delta = Physics::useFixedDeltatime ? Physics::physicsStepTime : (float)deltatime.count() / (float)1000;
			for (auto& collision : collisions)
			{
				delete collision;
			}
			collisions.clear();

			auto view = world.Registry().view<Position, Scale, RigidBody>();
			for (auto it = view.begin(); it != view.end(); it++){
				Position& pos = view.get<Position>(*it);
				Scale& scale = view.get<Scale>(*it);
				RigidBody& rb = view.get<RigidBody>(*it);
				if (rb.collider == nullptr) continue;
					for (auto otherIt = it; otherIt != view.end(); otherIt++) {
						if (otherIt == it) continue;
						Position& otherPos = view.get<Position>(*otherIt);
						Scale& otherScale = view.get<Scale>(*it);
						RigidBody& otherRb = view.get<RigidBody>(*otherIt);
						if (otherRb.collider == nullptr) continue;
						State a = { *it, pos, scale, &rb };
						State b = { *otherIt, otherPos, otherScale, &otherRb };
						if (rb.collider->GetShape() == otherRb.collider->GetShape()) {
							// Same Shape
							switch (rb.collider->GetShape())
							{
							case components::Shape::Box: {
								Collision* collision = IsBoxToBoxColliding(a, b);
								if (collision != nullptr) {
									collisions.push_back(collision);
								}
								break;
							}
							case components::Shape::Sphere: {
								Collision* collision = IsColliding(a, b);
								if (collision != nullptr) {
									collisions.push_back(collision);
								}
								break;
							}
							default:
								break;
							}
						}
						else {
							Collision* collision = nullptr;
							if (a.rb->collider->GetShape() == Shape::Box)
								collision = IsBoxToSphereColliding(a, b);
							else
								collision = IsBoxToSphereColliding(b, a);

							if (collision != nullptr) {
								collisions.push_back(collision);
							}
						}
					}
					// vérifie si collision avec le sol
					float groundPenetration = FLT_MAX;
					switch (rb.collider->GetShape())
					{
					case Shape::Sphere:
						groundPenetration = pos.local.y - ((SphereCollider*)rb.collider)->GetSphereData().radius;
						break;
					case Shape::Box:
						groundPenetration = pos.local.y - ((BoxCollider*)rb.collider)->shapeData.max.y;
						break;
					default:
						groundPenetration = FLT_MAX;
						break;
					}
					
					
					if (groundPenetration <= 0) {
						// Collision with ground
						entt::entity entites[2] = { *it, (entt::entity)-1 };
						RigidBody* rigidbodies[2] = { &rb, nullptr };
						groundPenetration = abs(groundPenetration);
						collisions.push_back(new Collision(entites, rigidbodies, rb.restitutionCoef, abs(groundPenetration), Vector3D(0, 1, 0)));
					}
				}
			resolver.resolveCollisions(collisions, delta);
		}
		// check sphere to sphere collision
		Collision* IsColliding(State a, State b) {
			SphereData aSphere = ((SphereCollider*)a.rb->collider)->GetSphereData();
			SphereData bSphere = ((SphereCollider*)b.rb->collider)->GetSphereData();
			Vector3D dir = a.pos.local.subVector(b.pos.local); // vecteur de direction (B vers A)
			float distance = abs(dir.magnitude()); //  distance absolue entre les deux particules
			float closestDistance = aSphere.radius + bSphere.radius; // plus proche distance possible
			if (distance <= closestDistance) {
				// collision
				entt::entity entites[2] = { a.entity, b.entity };
				RigidBody* rigidbodies[2] = { a.rb, b.rb };
				float restitutionCoef = 1;
				if (a.rb->restitutionCombine == b.rb->restitutionCombine) {
					restitutionCoef = (a.rb->restitutionCoef + b.rb->restitutionCoef) / 2;
				}

				float penetration = closestDistance - distance;

				std::cout << std::to_string(penetration) << std::endl;
				// on normalise le vecteur de direction
				;

				return new Collision(entites, rigidbodies, restitutionCoef, penetration, dir.normalize());
			}
			// No collision
			return nullptr;
		}

		struct AABB
		{
			Vector3D min;
			Vector3D max;
		};
		//Given point p, return the point q on or in AABB b that is closest to p
		Vector3D ClosestPtPointAABB(Vector3D p, AABB box)
		{
			Vector3D q;
			// For each coordinate axis, if the point coordinate value is
			// outside box, clamp it to the box, else keep it as is
			float v = p.x;
			if (v < box.min.x) v = box.min.x;
			if (v > box.max.x) v = box.max.x;
			q.x = v;
			v = p.y;
			if (v < box.min.y) v = box.min.y;
			if (v > box.max.y) v = box.max.y;
			q.y = v;
			v = p.z;
			if (v < box.min.z) v = box.min.z;
			if (v > box.max.z) v = box.max.z;
			q.z = v;
			return q;
		}
		Vector3D GetNormalAABBAABB(Vector3D vec) {
			Vector3D vecAbs = Vector3D(abs(vec.x), abs(vec.y), abs(vec.z));
			if (vecAbs.x > vecAbs.y && vecAbs.x > vecAbs.z) {
				return Vector3D(vec.x, 0, 0).normalize();
			}
			if (vecAbs.y > vecAbs.x && vecAbs.y > vecAbs.z) {
				return Vector3D(0, vec.y, 0).normalize();
			}
			if (vecAbs.z > vecAbs.x && vecAbs.z > vecAbs.y) {
				return Vector3D(0, 0, vec.z).normalize();
			}
		}
		// check box to box
		Collision* IsBoxToBoxColliding(State a, State b) {
			BoxData aBox = ((BoxCollider*)a.rb->collider)->GetBoxData();
			BoxData bBox = ((BoxCollider*)b.rb->collider)->GetBoxData();
			if (a.pos.local.x < b.pos.local.x + bBox.max.x * 2 &&
				a.pos.local.x + aBox.max.x * 2 > b.pos.local.x &&
				a.pos.local.y < b.pos.local.y + bBox.max.y * 2 &&
				a.pos.local.y + aBox.max.y * 2  > b.pos.local.y&&
				a.pos.local.z < b.pos.local.z + bBox.max.z * 2 &&
				a.pos.local.z + aBox.max.z * 2 > b.pos.local.z)
			{
				
				// collision
				entt::entity entites[2] = { a.entity, b.entity };
				RigidBody* rigidbodies[2] = { a.rb, b.rb };
				Vector3D pointtoA = ClosestPtPointAABB(a.pos.local, { bBox.min.sumVector(b.pos.local), bBox.max.sumVector(b.pos.local) });
				Vector3D pointToB = ClosestPtPointAABB(b.pos.local, { aBox.min.sumVector(a.pos.local), aBox.max.sumVector(a.pos.local) });
				Vector3D dir = GetNormalAABBAABB(a.pos.local.subVector(b.pos.local));
				float restitutionCoef = 1;
				if (a.rb->restitutionCombine == b.rb->restitutionCombine) {
					restitutionCoef = (a.rb->restitutionCoef + b.rb->restitutionCoef) / 2;
				}
				float penetration = pointtoA.VectorMultiplication(dir).subVector(pointToB.VectorMultiplication(dir)).magnitude();
				return new Collision(entites, rigidbodies, restitutionCoef, penetration, dir.normalize());
			}
			return nullptr;
		}
		// Returns the squared distance between a point p and an AABB b
		float SqDistPointAABB(Vector3D p, AABB box)
		{
			float sqDist = 0.0f;
			float v = p.x;
			if (v < box.min.x) sqDist += (box.min.x - v) * (box.min.x - v);
			if (v > box.max.x) sqDist += (v - box.max.x) * (v - box.max.x);
			v = p.y;
			if (v < box.min.y) sqDist += (box.min.y - v) * (box.min.y - v);
			if (v > box.max.y) sqDist += (v - box.max.y) * (v - box.max.y);
			v = p.z;
			if (v < box.min.z) sqDist += (box.min.z - v) * (box.min.z - v);
			if (v > box.max.z) sqDist += (v - box.max.z) * (v - box.max.z);
			return sqDist;
		}
		Collision* IsBoxToSphereColliding(State box, State sphere) {
			BoxData boxData = ((BoxCollider*)box.rb->collider)->GetBoxData();
			SphereData sphereData = ((SphereCollider*)sphere.rb->collider)->GetSphereData();
			Vector3D point = ClosestPtPointAABB(sphere.pos.local, { boxData.min.sumVector(box.pos.local), boxData.max.sumVector(box.pos.local) });
			Vector3D dir = point.subVector(sphere.pos.local);
			float dist = dir.magnitude();
			if (dist <= sphereData.radius) {
				float penetration = sphereData.radius - dist;
				// collision
				entt::entity entites[2] = { box.entity, sphere.entity };
				RigidBody* rigidbodies[2] = { box.rb, sphere.rb };

				float restitutionCoef = 1.0f;
				if (box.rb->restitutionCombine == sphere.rb->restitutionCombine) {
					restitutionCoef = (sphere.rb->restitutionCoef + box.rb->restitutionCoef) / 2;
				}
				return new Collision(entites, rigidbodies, restitutionCoef, penetration, dir.normalize());
			}
			return nullptr;
		}
		/*Collision* IsBoxToSphereColliding(State box, State sphere) {
			BoxData boxData = ((BoxCollider*)box.rb->collider)->GetBoxData();
			SphereData sphereData = ((SphereCollider*)sphere.rb->collider)->GetSphereData();
			// Find point p on AABB closest to sphere center
			Vector3D  point = ClosestPtPointAABB(sphere.pos.local, boxData);
			Vector3D dist = point.subVector(sphere.pos.local);
			// Sphere and AABB intersect if the (squared) distance between them is
			// less than the (squared) sphere radius.
			if (dist.scalarProduct(dist) <= sphereData.radius * sphereData.radius) {
				// collision
				entt::entity entites[2] = { box.entity, sphere.entity };
				RigidBody* rigidbodies[2] = { box.rb, sphere.rb };
				Vector3D dir = box.pos.local.subVector(sphere.pos.local).normalize();
				float restitutionCoef = 1;
				if (box.rb->restitutionCombine == sphere.rb->restitutionCombine) {
					restitutionCoef = (sphere.rb->restitutionCoef + box.rb->restitutionCoef) / 2;
				}
				float penetration = 0.0025f;
				return new Collision(entites, rigidbodies, restitutionCoef, penetration, dir);
			}
			else {
				return nullptr;
			}
		}*/
	private:
		std::chrono::milliseconds m_totalMilli = 0ms;
		std::vector<Collision*> collisions;
		CollisionResolver resolver;

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