#include <team4_game_engine/common.hpp>
#include <team4_game_engine/systems/physics_system.hpp>
#include <team4_game_engine/engine/physics/physics.hpp>
using namespace team4_game_engine::physics;
#include <spdlog/spdlog.h>
#include <team4_game_engine/engine/engine.hpp>
#include <team4_game_engine/engine/world.hpp>
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
using namespace team4_game_engine::engine::mathematics;
#include <team4_game_engine/components/position.hpp>
#include <team4_game_engine/components/rigidbody.hpp>
using namespace team4_game_engine::components;

namespace team4_game_engine::systems {
	class PhysicsSystemImpl {
	public:
		void Update(std::chrono::milliseconds deltatime, engine::World& world) {
			if (!Physics::doPhysicsStep && !Physics::doNextStep) return;
			if (Physics::useFixedDeltatime) {
				m_totalMilli += deltatime;
				if (m_totalMilli < 1s) return;
				m_totalMilli = 0ms;
			}
			float delta = Physics::useFixedDeltatime ? Physics::physicsStepTime : (float)deltatime.count()/ (float)1000;

			if (delta <= 0.0) return;
			IntegrateLoop(delta, world);
			forceLoop(delta, world);
		}

		void IntegrateLoop(float delta, engine::World& world) {
			world.Registry().view<Position, RigidBody>().each([this, delta](Position& pos, RigidBody& rb) {
					if (rb.inverseMass <= 0 || rb.isKinematic) return;

					assert(delta > 0.0);
					Vector3D position = pos.local.sumScalarVector(rb.velocity, delta);
					pos.local.x = position.x;
					pos.local.y = position.y;
					pos.local.z = position.z;

					Vector3D resultAcc = rb.acceleration;
					resultAcc = resultAcc.sumScalarVector(rb.accumulateForces, rb.inverseMass);

					//velocity = velocity.sumScalarVector(resultAcc, deltatime);

					rb.velocity = rb.velocity.sumScalarVector(resultAcc, delta);

					rb.accumulateForces = engine::mathematics::Vector3D();
				});
		}

		void forceLoop(float delta, engine::World& world) {
			world.Registry().view<RigidBody>().each([this, delta](RigidBody& rb) {
					//si la mass est null aucune force ne peut s'appliquer
					if (rb.inverseMass <= 0) return;
					if (rb.useGravity) {
						rb.accumulateForces = rb.accumulateForces.sumVector(rb.gravity.scalarMultiplication(1.0f / rb.inverseMass));
					}
					// Drag 
					float drag = std::clamp(1.0f - rb.drag * delta, 0.f, 1.f);
					rb.velocity = rb.velocity.scalarMultiplication(drag);
				});
		}
	private:
		std::chrono::milliseconds m_totalMilli = 0ms;
	};

	PhysicsSystem::PhysicsSystem() {
		m_impl = std::make_unique<PhysicsSystemImpl>();
	}

	PhysicsSystem::~PhysicsSystem() {
		m_impl = nullptr;
	}

	void PhysicsSystem::Update(std::chrono::milliseconds deltatime, engine::World& world) {
		m_impl->Update(deltatime, world);
	}
}