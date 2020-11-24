#pragma once

#include <team4_game_engine/engine/mathematics/vector3d.hpp>
#include <team4_game_engine/engine/mathematics/quaternion.hpp>
#include <team4_game_engine/engine/mathematics/matrix3.hpp>
#include <team4_game_engine/engine/mathematics/matrix4.hpp>
using namespace team4_game_engine::engine::mathematics;
#include <team4_game_engine/components/position.hpp>
#include <team4_game_engine/components/rotation.hpp>
#include <team4_game_engine/components/rigidbody.hpp>
using namespace team4_game_engine::components;

namespace team4_game_engine::physics {
	class Physics {
	public:
		static inline void _calculateTransformMatrix(Matrix4& transformMatrix, const Vector3D& position, const Quaternion& orientation)
		{
			transformMatrix.data[0] = 1 - 2 * orientation.j * orientation.j - 2 * orientation.k * orientation.k;
			transformMatrix.data[1] = 2 * orientation.i * orientation.j - 2 * orientation.w * orientation.k;
			transformMatrix.data[2] = 2 * orientation.i * orientation.k + 2 * orientation.w * orientation.j;
			transformMatrix.data[3] = position.x;

			transformMatrix.data[4] = 2 * orientation.i * orientation.j + 2 * orientation.w * orientation.k;
			transformMatrix.data[5] = 1 - 2 * orientation.i * orientation.i - 2 * orientation.k * orientation.k;
			transformMatrix.data[6] = 2 * orientation.j * orientation.k - 2 * orientation.w * orientation.i;
			transformMatrix.data[7] = position.y;

			transformMatrix.data[8] = 2 * orientation.i * orientation.k - 2 * orientation.w * orientation.j;
			transformMatrix.data[9] = 2 * orientation.j * orientation.k + 2 * orientation.w * orientation.i;
			transformMatrix.data[10] = 1 - 2 * orientation.i * orientation.i - 2 * orientation.j * orientation.j;
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

		static void CalculateDerivedData(Position& pos, Rotation& rot, RigidBody& rb)
		{
			rot.normalize();

			_calculateTransformMatrix(rb.transforMatrix, pos.local, rot);

			_transformInertiaTensor(rb.inverseInertiaTensorWorld, rot, rb.inverseInertiaTensor, rb.transforMatrix);
		}

		static void setInertiaTensor(RigidBody& rb, const Matrix3& inertiaTensor)
		{
			rb.inverseInertiaTensor.Setinverse(inertiaTensor);
		}

		static void AddForce(RigidBody& rb, const Vector3D& force)
		{
			rb.accumulateLinearForces = rb.accumulateLinearForces.sumVector(force);
			rb.isAwake = true;
		}

		static void AddForceAtBodyPoint(Position& pos, RigidBody& rb, const Vector3D& force, const Vector3D& point)
		{
			Vector3D pt = Vector3D().localToWorld(point, rb.transforMatrix);
			AddForceAtPoint(pos, rb, force, pt);

			rb.isAwake = true;
		}

		static void AddForceAtPoint(Position& pos, RigidBody& rb, const Vector3D& force, const Vector3D& point)
		{
			Vector3D pt = point;
			pt = pt.subVector(pos.local.sumVector(rb.massCenter));

			rb.accumulateLinearForces = rb.accumulateLinearForces.sumVector(force);
			rb.accumulateAngularForces = rb.accumulateAngularForces.sumVector(pt.vectorProduct(force));

			rb.isAwake = true;
		}
		static bool doPhysicsStep;
		static bool doCollisionStep;
		static bool doNextStep;
		static bool useFixedDeltatime;
		static int physicsStepPerSecond;
		static float physicsStepTime;
	};
}