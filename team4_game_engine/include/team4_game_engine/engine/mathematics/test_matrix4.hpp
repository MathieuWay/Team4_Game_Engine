#pragma once
#include <team4_game_engine/engine/mathematics/matrix4.hpp>
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
#include <team4_game_engine/engine/mathematics/quaternion.hpp>

namespace team4_game_engine::engine::mathematics {
	void testMatrix4() {
		std::cout << "Matrice4 m : " << std::endl;
		Matrix4 m = Matrix4();
		m.printMatrix4();

		std::cout << "Quaernions q : " << std::endl;
		Quaternions q = Quaternions(1, 1, 1, 1);
		q.printQuaternions();

		std::cout << "vector v : " << std::endl;
		Vector3D v = Vector3D(1, 1, 1);
		v.printVector();

		std::cout << "setorientationandpos sur m avec q et v " << std::endl;
		m.setOrientationAndPos(q, v);
		m.printMatrix4();

		std::cout << "matrix4Multiplication n = m*m " << std::endl;
		Matrix4 n = m.matrix4Multiplication(m);
		n.printMatrix4();

		std::cout << "determinant de m " << std::endl;
		float det = m.getDeterminant();
		std::cout << det << std::endl;

		std::cout << "inverse de m " << std::endl;
		n = m.inverse();
		n.printMatrix4();

		std::cout << "vector v : " << std::endl;
		v = Vector3D(2, 2, 2);
		v.printVector();

		std::cout << "vector u =  localToWorld(v,m);" << std::endl;
		Vector3D u = localToWorld(v, m);
		u.printVector();

		std::cout << "vector u =  worldToLocal(v,m);" << std::endl;
		u = worldToLocal(v, m);
		u.printVector();

		std::cout << "vector u =  localToWorldDirn(v,m);" << std::endl;
		u = localToWorldDirn(v, m);
		u.printVector();

		std::cout << "vector u =  worldToLocalDirn(v,m);" << std::endl;
		u = worldToLocalDirn(v, m);
		u.printVector();
	}
}