#include <team4_game_engine/common.hpp>
#include <team4_game_engine/systems/physics_system.hpp>
#include <team4_game_engine/engine/physics/physics.hpp>
using namespace team4_game_engine::physics;
#include <spdlog/spdlog.h>
#include <team4_game_engine/engine/engine.hpp>
#include <team4_game_engine/engine/world.hpp>
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
#include <team4_game_engine/engine/mathematics/quaternion.hpp>
#include <team4_game_engine/engine/mathematics/matrix3.hpp>
#include <team4_game_engine/engine/mathematics/matrix4.hpp>
using namespace team4_game_engine::engine::mathematics;
#include <team4_game_engine/components/position.hpp>
#include <team4_game_engine/components/rotation.hpp>
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
			forceLoop(delta, world);
			IntegrateLoop(delta, world);
		}
	private:
		std::chrono::milliseconds m_totalMilli = 0ms;

		void IntegrateLoop(float delta, engine::World& world) {
			world.Registry().view<Position, Rotation, RigidBody>().each([this, delta](Position& pos, Rotation& rot, RigidBody& rb) {
				if (rb.inverseMass <= 0 || rb.isKinematic) return;

				assert(delta > 0.0);

				Vector3D resultAccL = Vector3D().sumScalarVector(rb.accumulateLinearForces, rb.inverseMass);
				Vector3D resultAccA = rb.inverseInertiaTensorWorld * rb.accumulateAngularForces;

				rb.linearVelocity = rb.linearVelocity.sumScalarVector(resultAccL, delta);
				rb.angularVelocity = rb.angularVelocity.sumScalarVector(resultAccA, delta);

				rb.linearVelocity = rb.linearVelocity.scalarMultiplication(powf(rb.linearDrag, delta));
				rb.angularVelocity = rb.angularVelocity.scalarMultiplication(powf(rb.angularDrag, delta));

				pos.local = pos.local.sumScalarVector(rb.linearVelocity, delta);
				rot.addScaledVector(rb.angularVelocity, delta);

				Physics::CalculateDerivedData(pos, rot, rb);

				//Clear Accumulators
				rb.accumulateLinearForces = engine::mathematics::Vector3D();
				rb.accumulateAngularForces = engine::mathematics::Vector3D();
				});
		}

		void forceLoop(float delta, engine::World& world) {
			world.Registry().view<RigidBody>().each([this, delta](RigidBody& rb) {
				//si la mass est null aucune force ne peut s'appliquer
				if (rb.inverseMass <= 0) return;
				if (rb.useGravity) {
					rb.accumulateLinearForces = rb.accumulateLinearForces.sumVector(rb.gravity.scalarMultiplication(1.0f / rb.inverseMass));
				}
				// Drag 
				/*float linearDrag = std::clamp(1.0f - rb.linearDrag * delta, 0.f, 1.f);
				float angularDrag = std::clamp(1.0f - rb.angularDrag * delta, 0.f, 1.f);
				rb.linearVelocity = rb.linearVelocity.scalarMultiplication(linearDrag);
				rb.angularVelocity = rb.angularVelocity.scalarMultiplication(angularDrag);*/
				});
		}
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