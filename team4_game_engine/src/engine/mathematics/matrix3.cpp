#include <team4_game_engine/engine/mathematics/matrix3.hpp>
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
#include <team4_game_engine/engine/mathematics/quaternion.hpp>
#include <iostream>

namespace team4_game_engine::engine::mathematics {

	// Constructor + Destructor.

	Matrix3::Matrix3()
	{
		this->data[0] = 0; this->data[1] = 0; this->data[2] = 0; this->data[3] = 0; this->data[4] = 0;
		this->data[5] = 0; this->data[6] = 0; this->data[7] = 0; this->data[8] = 0;
	}

	Matrix3::Matrix3(float d1, float d2, float d3, float d4, float d5, float d6, float d7, float d8, float d9)
	{
		this->data[0] = d1; this->data[1] = d2; this->data[2] = d3; this->data[3] = d4; this->data[4] = d5;
		this->data[5] = d6; this->data[6] = d7; this->data[7] = d8; this->data[8] = d9;
	}

	Matrix3::~Matrix3()
	{
	}


	// Transform given Vector by the Matrix.

	Vector3D Matrix3::operator*(Vector3D vector)
	{
		return Vector3D(
			vector.x * data[0] + vector.y * data[1] + vector.z * data[2],
			vector.x * data[3] + vector.y * data[4] + vector.z * data[5],
			vector.x * data[6] + vector.y * data[7] + vector.z * data[8]
		);
	}

	// Return a Matrix who is the mutliplication of this one with an another that is given.
	Matrix3 Matrix3::operator*(Matrix3 M)
	{
		return Matrix3(
			data[0] * M.data[0] + data[1] * M.data[3] + data[2] * M.data[6],
			data[0] * M.data[1] + data[1] * M.data[4] + data[2] * M.data[7],
			data[0] * M.data[2] + data[1] * M.data[5] + data[3] * M.data[8],

			data[3] * M.data[0] + data[4] * M.data[3] + data[5] * M.data[6],
			data[3] * M.data[1] + data[4] * M.data[4] + data[5] * M.data[7],
			data[3] * M.data[2] + data[4] * M.data[5] + data[5] * M.data[8],

			data[6] * M.data[0] + data[7] * M.data[3] + data[8] * M.data[6],
			data[6] * M.data[1] + data[7] * M.data[4] + data[8] * M.data[7],
			data[6] * M.data[2] + data[7] * M.data[5] + data[8] * M.data[8]
		);
	}

	// Multiply this Matrix whit a given one.
	void Matrix3::operator*=(Matrix3 M)
	{
		float t1;
		float t2;
		float t3;

		t1 = data[0] * M.data[0] + data[1] * M.data[3] + data[2] * M.data[6];
		t2 = data[0] * M.data[1] + data[1] * M.data[4] + data[2] * M.data[7];
		t3 = data[0] * M.data[2] + data[1] * M.data[5] + data[3] * M.data[8];
		data[0] = t1;
		data[1] = t2;
		data[2] = t3;

		t1 = data[3] * M.data[0] + data[4] * M.data[3] + data[5] * M.data[6];
		t2 = data[3] * M.data[1] + data[4] * M.data[4] + data[5] * M.data[7];
		t3 = data[3] * M.data[2] + data[4] * M.data[5] + data[5] * M.data[8];
		data[3] = t1;
		data[4] = t2;
		data[5] = t3;

		t1 = data[6] * M.data[0] + data[7] * M.data[3] + data[8] * M.data[6];
		t2 = data[6] * M.data[1] + data[7] * M.data[4] + data[8] * M.data[7];
		t3 = data[6] * M.data[2] + data[7] * M.data[5] + data[8] * M.data[8];
		data[6] = t1;
		data[7] = t2;
		data[8] = t3;
	}

	// Set the Matrix to be the inverse of a given Matrix.
	void Matrix3::Setinverse(Matrix3 M)
	{
		float t1 = M.data[0] * M.data[4];
		float t2 = M.data[0] * M.data[5];
		float t3 = M.data[1] * M.data[3];
		float t4 = M.data[2] * M.data[3];
		float t5 = M.data[1] * M.data[6];
		float t6 = M.data[2] * M.data[6];

		// Calculate the determinant and make sure that it is !=0.
		float det = (t1 * M.data[8] - t2 * M.data[7] - t3 * M.data[8] +
			t4 * M.data[7] + t5 * M.data[5] - t6 * M.data[4]);
		if (det == 0.0f) return;
		float invDet = 1.0f / det;

		data[0] = (M.data[4] * M.data[8] - M.data[5] * M.data[7]) * invDet;
		data[1] = -(M.data[1] * M.data[8] - M.data[2] * M.data[7]) * invDet;
		data[2] = (M.data[1] * M.data[5] - M.data[2] * M.data[4]) * invDet;
		data[3] = -(M.data[3] * M.data[8] - M.data[5] * M.data[6]) * invDet;
		data[4] = (M.data[0] * M.data[8] - t6) * invDet;
		data[5] = -(t2 - t4) * invDet;
		data[6] = (M.data[3] * M.data[7] - M.data[4] * M.data[6]) * invDet;
		data[7] = -(M.data[0] * M.data[7] - t5) * invDet;
		data[8] = (t1 - t3) * invDet;
	}

	Matrix3 Matrix3::Inverse()
	{
		Matrix3 result;
		result.Setinverse(*this);
		return result;
	}

	void Matrix3::Invert()
	{
		Setinverse(*this);
	}

	// Set the Matrix to be the transpose of the given Matrix.
	void Matrix3::SetTranspose(Matrix3 M)
	{
		data[0] = M.data[0];
		data[1] = M.data[3];
		data[2] = M.data[6];
		data[3] = M.data[1];
		data[4] = M.data[4];
		data[5] = M.data[7];
		data[6] = M.data[2];
		data[7] = M.data[5];
		data[8] = M.data[8];
	}

	Matrix3 Matrix3::Transpose()
	{
		Matrix3 result;
		result.SetTranspose(*this);
		return result;
	}

	// Make the rotation matrix corresponding to the Quaternion given.
	void Matrix3::SetOrientation(Quaternion Q)
	{
		data[0] = 1 - (2 * Q.j * Q.j + 2 * Q.k * Q.k);
		data[1] = 2 * Q.i * Q.j + 2 * Q.k * Q.w;
		data[2] = 2 * Q.i * Q.k - 2 * Q.j * Q.w;
		data[3] = 2 * Q.i * Q.j - 2 * Q.k * Q.w;
		data[4] = 1 - (2 * Q.i * Q.i + 2 * Q.k * Q.k);
		data[5] = 2 * Q.j * Q.k + 2 * Q.i * Q.w;
		data[6] = 2 * Q.i * Q.k + 2 * Q.j * Q.w;
		data[7] = 2 * Q.j * Q.k - 2 * Q.i * Q.w;
		data[8] = 1 - (2 * Q.i * Q.i + 2 * Q.j * Q.j);
	}

}

