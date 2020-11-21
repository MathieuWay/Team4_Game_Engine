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
		void IntegrateLoop(float delta, engine::World& world) {
			world.Registry().view<Position, Rotation, RigidBody>().each([this, delta](Position& pos, Rotation& rot, RigidBody& rb) {
				if (rb.inverseMass <= 0 || rb.isKinematic) return;

				assert(delta > 0.0);
				Vector3D resultAccL = rb.acceleration;
				resultAccL = resultAccL.sumScalarVector(rb.accumulateLinearForces, rb.inverseMass);
				Vector3D resultAccA = rb.inverseInertiaTensorWorld * rb.accumulateAngularForces;

				rb.velocity = rb.velocity.sumScalarVector(resultAccL, delta);
				rb.angularVelocity = rb.angularVelocity.sumScalarVector(resultAccA, delta);

				pos.local = pos.local.sumScalarVector(rb.velocity, delta);
				rot.addScaledVector(rb.angularVelocity, delta);

				CalculateDerivedData(pos, rot, rb);

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
				float linearDrag = std::clamp(1.0f - rb.linearDrag * delta, 0.f, 1.f);
				float angularDrag = std::clamp(1.0f - rb.angularDrag * delta, 0.f, 1.f);
				rb.velocity = rb.velocity.scalarMultiplication(linearDrag);
				rb.angularVelocity = rb.angularVelocity.scalarMultiplication(angularDrag);
				});
		}

		static inline void _calculateTransformMatrix(Matrix4& transformMatrix, const Vector3D& position, const Quaternion& orientation)
		{
			transformMatrix.data[0] = 1 - (2 * orientation.j * orientation.j - 2 * orientation.k * orientation.k);
			transformMatrix.data[1] = 2 * orientation.i * orientation.j - 2 * orientation.w * orientation.k;
			transformMatrix.data[2] = 2 * orientation.i * orientation.k + 2 * orientation.w * orientation.j;
			transformMatrix.data[3] = position.x;

			transformMatrix.data[4] = 2 * orientation.i * orientation.j + 2 * orientation.w * orientation.k;
			transformMatrix.data[5] = 1 - (2 * orientation.i * orientation.i - 2 * orientation.k * orientation.k);
			transformMatrix.data[6] = 2 * orientation.j * orientation.k - 2 * orientation.w * orientation.i;
			transformMatrix.data[7] = position.y;

			transformMatrix.data[8] = 2 * orientation.i * orientation.k - 2 * orientation.w * orientation.j;
			transformMatrix.data[9] = 2 * orientation.j * orientation.k + 2 * orientation.w * orientation.i;
			transformMatrix.data[10] = 1 - (2 * orientation.i * orientation.i - 2 * orientation.j * orientation.j);
			transformMatrix.data[11] = position.z;
		}

		static inline void _transformInertiaTensor(Matrix3& iitWorld, const Quaternion& q, const Matrix3& iitBody, const Matrix4& rotMat)
		{
			float t4 = rotMat.data[0] * iitBody.data[0] + rotMat.data[1] * iitBody.data[3] + rotMat.data[2] * iitBody.data[6];
			float t9 = rotMat.data[0] * iitBody.data[1] + rotMat.data[1] * iitBody.data[4] + rotMat.data[2] * iitBody.data[7];
			float t14 = rotMat.data[0] * iitBody.data[2] + rotMat.data[1] * iitBody.data[5] + rotMat.data[2] * iitBody.data[8];

			float t28 = rotMat.data[4] * iitBody.data[0] + rotMat.data[5] * iitBody.data[3] + rotMat.data[6] * iitBody.data[6];
			float t33 = rotMat.data[4] * iitBody.data[1] + rotMat.data[5] * iitBody.data[4] + rotMat.data[6] * iitBody.data[7];
			float t38 = rotMat.data[4] * iitBody.data[2] + rotMat.data[5] * iitBody.data[5] + rotMat.data[6] * iitBody.data[8];

			float t52 = rotMat.data[8] * iitBody.data[0] + rotMat.data[9] * iitBody.data[3] + rotMat.data[10] * iitBody.data[6];
			float t57 = rotMat.data[8] * iitBody.data[1] + rotMat.data[9] * iitBody.data[4] + rotMat.data[10] * iitBody.data[7];
			float t62 = rotMat.data[8] * iitBody.data[2] + rotMat.data[9] * iitBody.data[5] + rotMat.data[10] * iitBody.data[8];

			iitWorld.data[0] = t4 * rotMat.data[0] + t9 * rotMat.data[1] + t14 * rotMat.data[2];
			iitWorld.data[1] = t4 * rotMat.data[4] + t9 * rotMat.data[5] + t14 * rotMat.data[6];
			iitWorld.data[2] = t4 * rotMat.data[8] + t9 * rotMat.data[9] + t14 * rotMat.data[10];

			iitWorld.data[3] = t28 * rotMat.data[0] + t33 * rotMat.data[1] + t38 * rotMat.data[2];
			iitWorld.data[4] = t28 * rotMat.data[4] + t33 * rotMat.data[5] + t38 * rotMat.data[6];
			iitWorld.data[5] = t28 * rotMat.data[8] + t33 * rotMat.data[9] + t38 * rotMat.data[10];

			iitWorld.data[6] = t52 * rotMat.data[0] + t57 * rotMat.data[1] + t62 * rotMat.data[2];
			iitWorld.data[7] = t52 * rotMat.data[4] + t57 * rotMat.data[5] + t62 * rotMat.data[6];
			iitWorld.data[8] = t52 * rotMat.data[8] + t57 * rotMat.data[9] + t62 * rotMat.data[10];
		}

		void CalculateDerivedData(Position& pos, Rotation& rot, RigidBody& rb)
		{
			rot.normalize();

			_calculateTransformMatrix(rb.transforMatrix, pos.local, rot);

			_transformInertiaTensor(rb.inverseInertiaTensorWorld, rot, rb.inverseInertiaTensor, rb.transforMatrix);
		}

		void setInertiaTensor(RigidBody& rb, const Matrix3& inerciaTensor)
		{
			rb.inverseInertiaTensor.Setinverse(inerciaTensor);
		}

		void AddForce(RigidBody& rb, const Vector3D& force)
		{
			rb.accumulateLinearForces = rb.accumulateLinearForces.sumVector(force);
			rb.isAwake = true;
		}

		void AddForceAtBodyPoint(Position& pos, RigidBody& rb, const Vector3D& force, const Vector3D& point)
		{
			Vector3D pt = Vector3D().localToWorld(point, rb.transforMatrix);
			AddForceAtPoint(pos, rb, force, pt);

			rb.isAwake = true;
		}

		void AddForceAtPoint(Position& pos, RigidBody& rb, const Vector3D& force, const Vector3D& point)
		{
			Vector3D pt = point;
			pt = pt.subVector(pos.local);

			rb.accumulateLinearForces = rb.accumulateLinearForces.sumVector(force);
			rb.accumulateAngularForces = rb.accumulateAngularForces.sumVector(pt.VectorMultiplication(force));

			rb.isAwake = true;
		}
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