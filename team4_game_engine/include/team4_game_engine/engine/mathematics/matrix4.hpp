#pragma once

namespace team4_game_engine::engine::mathematics {
	class Quaternion;
	class Vector3D;
	class Matrix4
	{
	public:
		/*
		contient les valeurs de la matrice 4x4
		/*/
		float data[12];

		Matrix4();
		~Matrix4();

		/*
		fait la multiplication matriciel entre la matrice et celle donnee en argument
		et renvoie le résultat dans une nouvelle matrice.
		*/
		Matrix4 matrix4Multiplication(const Matrix4& m);

		/*
		Calcul et renvoie le determinant de la matrice
		*/
		float getDeterminant() const;

		/*
		transforme la matrice en l'inverse de la matrice donnee en argument
		*/
		void setInverse(const Matrix4& m);

		Matrix4 inverse() const;

		/*
		transforme la matrice en matrice de rotation qui correspond au quaternion en argument
		*/
		void setOrientationAndPos(const Quaternion& q, const Vector3D& pos);

		/*
		transforme le Vecteur3D avec la matrice
		*/
		Vector3D transform(const Vector3D& vector) const;

		/*
		transforme le Vecteur3D avec l'inverse de la matrice
		*/
		Vector3D transformInverse(const Vector3D& vector) const;

		/*
		transforme le Vecteur3D de direction avec la matrice
		*/
		Vector3D transformDirection(const Vector3D& vector) const;

		/*
		transforme le Vecteur3D de direction avec l'inverse de la matrice
		*/
		Vector3D transformInverseDirection(const Vector3D& vector) const;

		Vector3D localToWorld(const Vector3D& local, const Matrix4& transform);

		Vector3D worldToLocal(const Vector3D& world, const Matrix4& transform);

		Vector3D localToWorldDirn(const Vector3D& local, const Matrix4& transform);

		Vector3D worldToLocalDirn(const Vector3D& world, const Matrix4& transform);

		/*
		affiche la matrcie 4
		*/
		void printMatrix4();
	};
}