#include <team4_game_engine/engine/mathematics/matrix4.hpp>
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
#include <team4_game_engine/engine/mathematics/quaternion.hpp>
#include <iostream>

namespace team4_game_engine::engine::mathematics {
	Matrix4::Matrix4()
	{
		for (size_t i = 0; i < 12; i++)
		{
			this->data[i] = 0;
		}
	}

	Matrix4::~Matrix4()
	{
	}

	/*
	fait la multiplication matriciel entre la matrice et celle donnee en argument
	et renvoie le résultat dans une nouvelle matrice.
	*/

	Matrix4 Matrix4::matrix4Multiplication(const Matrix4& m) {
		Matrix4 result;
		result.data[0] = m.data[0] * data[0] + m.data[4] * data[1] + m.data[8] * data[2];

		result.data[4] = m.data[0] * data[4] + m.data[4] * data[5] + m.data[8] * data[6];

		result.data[8] = m.data[0] * data[8] + m.data[4] * data[9] + m.data[8] * data[10];


		result.data[1] = m.data[1] * data[0] + m.data[5] * data[1] + m.data[9] * data[2];

		result.data[5] = m.data[1] * data[4] + m.data[5] * data[5] + m.data[9] * data[6];

		result.data[9] = m.data[1] * data[8] + m.data[5] * data[9] + m.data[9] * data[10];


		result.data[2] = m.data[2] * data[0] + m.data[6] * data[1] + m.data[10] * data[2];

		result.data[6] = m.data[2] * data[4] + m.data[6] * data[5] + m.data[10] * data[6];

		result.data[10] = m.data[2] * data[8] + m.data[6] * data[9] + m.data[10] * data[10];


		result.data[3] = m.data[3] * data[0] + m.data[7] * data[1] + m.data[11] * data[2] + data[3];

		result.data[7] = m.data[3] * data[4] + m.data[7] * data[5] + m.data[11] * data[6] + data[7];

		result.data[11] = m.data[3] * data[8] + m.data[7] * data[9] + m.data[11] * data[10] + data[11];

		return result;
	}

	/*
	Calcul et renvoie le determinant de la matrice
	*/
	float Matrix4::getDeterminant() const {
		return  data[0] * data[5] * data[10]
			+ data[4] * data[9] * data[2]
			+ data[8] * data[1] * data[6]
			- data[0] * data[9] * data[6]
			- data[4] * data[1] * data[10]
			- data[8] * data[5] * data[2];
	}

	/*
	transforme la matrice en l'inverse de la matrice donnee en argument
	*/
	void Matrix4::setInverse(const Matrix4& m) {
		/*
		si le determinant est egale ?0 la matrice n'est pas inversible
		*/
		float det = m.getDeterminant();
		if (det == 0) {
			return;
		}
		det = 1.0 / det;

		data[0] = (-m.data[9] * m.data[6] + m.data[5] * m.data[10]) * det;
		data[4] = (m.data[8] * m.data[6] - m.data[4] * m.data[10]) * det;
		data[8] = (-m.data[8] * m.data[5] + m.data[4] * m.data[9]) * det;

		data[1] = (m.data[9] * m.data[2] - m.data[1] * m.data[10]) * det;
		data[5] = (-m.data[8] * m.data[2] + m.data[0] * m.data[10]) * det;
		data[9] = (m.data[8] * m.data[1] - m.data[0] * m.data[9]) * det;

		data[2] = (-m.data[5] * m.data[2] + m.data[1] * m.data[6]) * det;
		data[6] = (m.data[4] * m.data[2] - m.data[0] * m.data[6]) * det;
		data[10] = (-m.data[4] * m.data[1] + m.data[0] * m.data[5]) * det;

		data[3] = (m.data[9] * m.data[6] * m.data[3]
			- m.data[5] * m.data[10] * m.data[3]
			- m.data[9] * m.data[2] * m.data[7]
			+ m.data[1] * m.data[10] * m.data[7]
			+ m.data[5] * m.data[2] * m.data[11]
			- m.data[1] * m.data[6] * m.data[11]) * det;
		data[7] = (-m.data[8] * m.data[6] * m.data[3]
			+ m.data[4] * m.data[10] * m.data[3]
			+ m.data[8] * m.data[2] * m.data[7]
			- m.data[0] * m.data[10] * m.data[7]
			- m.data[4] * m.data[2] * m.data[11]
			+ m.data[0] * m.data[6] * m.data[11]) * det;
		data[11] = (m.data[8] * m.data[5] * m.data[3]
			- m.data[4] * m.data[9] * m.data[3]
			- m.data[8] * m.data[1] * m.data[7]
			+ m.data[0] * m.data[9] * m.data[7]
			+ m.data[4] * m.data[1] * m.data[11]
			- m.data[0] * m.data[5] * m.data[11]) * det;
	}

	Matrix4 Matrix4::inverse() const {
		Matrix4 result;
		result.setInverse(*this);
		return result;
	}

	/*
	transforme la matrice en matrice de rotation qui correspond au quaternion en argument
	*/
	void Matrix4::setOrientationAndPos(const Quaternion& q, const Vector3D& pos) {
		data[0] = 1 - 2 * (q.j * q.j + q.k * q.k);
		data[1] = 2 * (q.i * q.j + q.k * q.w);
		data[2] = 2 * (q.i * q.k - q.j * q.w);

		data[4] = 2 * (q.i * q.j - q.k * q.w);
		data[5] = 1 - 2 * (q.i * q.i + q.k * q.k);
		data[6] = 2 * (q.j * q.k + q.i * q.w);

		data[8] = 2 * (q.i * q.k + q.j * q.w);
		data[9] = 2 * (q.j * q.k - q.i * q.w);
		data[10] = 1 - 2 * (q.i * q.i + q.j * q.j);

		data[3] = pos.x;
		data[7] = pos.y;
		data[11] = pos.z;
	}

	Vector3D Matrix4::transform(const Vector3D& vector) const
	{
		return Vector3D(
			vector.x * data[0] +
			vector.y * data[1] +
			vector.z * data[2] + data[3],

			vector.x * data[4] +
			vector.y * data[5] +
			vector.z * data[6] + data[7],

			vector.x * data[8] +
			vector.y * data[9] +
			vector.z * data[10] + data[11]
		);
	}

	/*
	transforme le Vecteur3D avec l'inverse de la matrice
	*/
	Vector3D Matrix4::transformInverse(const Vector3D& vector) const {
		Vector3D tmpV = vector;
		tmpV.x -= data[3];
		tmpV.y -= data[7];
		tmpV.z -= data[11];

		return Vector3D(
			tmpV.x * data[0] +
			tmpV.y * data[4] +
			tmpV.z * data[8],

			tmpV.x * data[1] +
			tmpV.y * data[5] +
			tmpV.z * data[9],

			tmpV.x * data[2] +
			tmpV.y * data[6] +
			tmpV.z * data[10]
		);
	}

	/*
	transforme le Vecteur3D de direction avec la matrice
	*/
	Vector3D Matrix4::transformDirection(const Vector3D& vector) const {
		return Vector3D(
			vector.x * data[0] +
			vector.y * data[1] +
			vector.z * data[2],

			vector.x * data[4] +
			vector.y * data[5] +
			vector.z * data[6],

			vector.x * data[8] +
			vector.y * data[9] +
			vector.z * data[10]
		);
	}

	/*
	transforme le Vecteur3D de direction avec l'inverse de la matrice
	*/
	Vector3D Matrix4::transformInverseDirection(const Vector3D& vector) const
	{
		return Vector3D(
			vector.x * data[0] +
			vector.y * data[4] +
			vector.z * data[8],

			vector.x * data[1] +
			vector.y * data[5] +
			vector.z * data[9],

			vector.x * data[2] +
			vector.y * data[6] +
			vector.z * data[10]
		);
	}
	/*
	permet de passer un Vector3D d'un repere local vers un repere global
	*/
	Vector3D Matrix4::localToWorld(const Vector3D& local, const Matrix4& transform) {
		return transform.transform(local);
	}

	/*
	permet de passer un Vector3D d'un repere global vers un repere local
	*/
	Vector3D Matrix4::worldToLocal(const Vector3D& world, const Matrix4& transform) {
		return transform.transformInverse(world);
	}

	/*
	permet de passer un Vector3D de direction d'un repere local vers un repere global
	*/
	Vector3D Matrix4::localToWorldDirn(const Vector3D& local, const Matrix4& transform) {
		return transform.transformDirection(local);
	}

	/*
	permet de passer un Vector3D de direction d'un repere global vers un repere local
	*/
	Vector3D Matrix4::worldToLocalDirn(const Vector3D& world, const Matrix4& transform) {
		return transform.transformInverseDirection(world);
	}

	/*
	affiche la Matrix4
	*/
	void Matrix4::printMatrix4() {
		std::cout << "m1 = " << data[0] << " m2 = " << data[1] << " m3 = " << data[2] << " m4 = " << data[3] << std::endl;
		std::cout << "m5 = " << data[4] << " m6 = " << data[5] << " m7 = " << data[6] << " m8 = " << data[7] << std::endl;
		std::cout << "m9 = " << data[8] << " m10 = " << data[9] << " m11 = " << data[10] << " m12 = " << data[11] << std::endl;
		std::cout << "m13 = " << 0 << " m14 = " << 0 << " m15 = " << 0 << " m16 = " << 1 << std::endl;
	}
}