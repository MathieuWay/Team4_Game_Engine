#include <team4_game_engine/engine/mathematics/quaternion.hpp>
#include <team4_game_engine/engine/mathematics/vector3d.hpp>

namespace team4_game_engine::engine::mathematics {
void testQuaternion() {
	
	Quaternion q = Quaternion();
	std::cout << "Quaernions q : " << std::endl;
	q.printQuaternion();
	Quaternion r = Quaternion(3, 5, 4, 2);

	std::cout << "Quaernions r : " << std::endl;
	r.printQuaternion();

	std::cout << " normalize Quaernions q : " << std::endl;
	q.normalize();
	q.printQuaternion();
	

	std::cout << "normalize Quaernions r : " << std::endl;
	r.normalize();
	r.printQuaternion();
	std::cout << "magnitude r :" << r.magnitude() << std::endl;

	std::cout << "Quaernions q : " << std::endl;
	q = Quaternion(1, 1, 1, 1);
	q.printQuaternion();
	
	std::cout << "Quaernions r : " << std::endl;
	r = Quaternion(2, 2, 2, 2);
	r.printQuaternion();

	r.hamiltonianProduct(q);
	std::cout << "hamlitonianProduct r * q : " << std::endl;
	r.printQuaternion();

	Vector3D v = Vector3D(1, 1, 1);
	std::cout << "vector v : " << std::endl;
	v.printVector();

	q.rotateByVector(v);
	std::cout << "rotate q with v : " << std::endl;
	q.printQuaternion();

	std::cout << "Quaernions q : " << std::endl;
	q = Quaternion(1, 1, 1, 1);
	q.printQuaternion();

	q.addScaledVector(v, 2);
	std::cout << "addScaledVector r * 2*v/2 : " << std::endl;
	q.printQuaternion();
}}