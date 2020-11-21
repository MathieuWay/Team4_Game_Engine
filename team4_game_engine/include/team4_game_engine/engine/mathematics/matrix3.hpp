#pragma once

namespace team4_game_engine::engine::mathematics {
	class Quaternion;
	class Vector3D;
	class Matrix3
	{
	public:
		// The Matrix data in a Array.
		float data[9];

		// constructor + Destructor
		Matrix3();
		Matrix3(float d1, float d2, float d3, float d4, float d5, float d6, float d7, float d8, float d9);
		~Matrix3();

		// Multiply the Matrix.
		Vector3D operator*(Vector3D vector);
		Matrix3 operator*(Matrix3 M);
		void operator*=(Matrix3 M);

		// Inverse the Matrix
		void Setinverse(Matrix3 M);
		Matrix3 Inverse();
		void Invert();

		// Transpose the Matrix
		void SetTranspose(Matrix3 M);
		Matrix3 Transpose();

		// Set the rotation Matrix
		void SetOrientation(Quaternion Q);
	};
}

