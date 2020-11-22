#include <team4_game_engine/engine/mathematics/quaternion.hpp>
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
#include <team4_game_engine/serialization/input_memory_stream.hpp>
#include <team4_game_engine/serialization/output_memory_stream.hpp>
#include <iostream>

namespace team4_game_engine::engine::mathematics {
	/*
		creer un quaternion avec des valeurs à 0
		*/
	Quaternion::Quaternion() {
		this->w = 0;
		this->i = 0;
		this->j = 0;
		this->k = 0;
		for (int i = 0; i < 4; i++) {
			this->data[i] = 0;
		}
	}

	/*
	creer un quaternion avec les valeurs passe en argument
	*/
	Quaternion::Quaternion(float w, float i, float j, float k) {

		this->w = w;
		this->data[0] = w;

		this->i = i;
		this->data[1] = i;

		this->j = j;
		this->data[2] = j;

		this->k = k;
		this->data[3] = k;
	}

	Quaternion::~Quaternion() {
	}

	/*
	normalise le quaternion
	*/
	void Quaternion::normalize() {

		float d = this->magnitude();

		if (d == 0) {
			w = 1;
			return;
		}

		d = (float(1.0)) / d;

		w *= d;
		i *= d;
		j *= d;
		k *= d;

		data[0] = w;
		data[1] = i;
		data[2] = j;
		data[3] = k;
	}

	/*
	fait le produit hamiltonien du quaternion et du quaternion passe en argument
	*/
	void Quaternion::hamiltonianProduct(const Quaternion q) {

		float wbis = w * q.w - i * q.i - j * q.j - k * q.k;

		float ibis = w * q.i + i * q.w + j * q.k - k * q.j;

		float jbis = w * q.j - i * q.k + j * q.w + k * q.i;

		float kbis = w * q.k + i * q.j - j * q.i + k * q.w;

		w = wbis;
		i = ibis;
		j = jbis;
		k = kbis;

		data[0] = w;
		data[1] = i;
		data[2] = j;
		data[3] = k;
	}

	/*
	inverse le quaternion en argument
	*/
	void Quaternion::invert() {
		w = -w;
		i = -i;
		j = -j;
		k = -k;

		data[0] = w;
		data[1] = i;
		data[2] = j;
		data[3] = k;
	}

	/*
	renvoie la norme du quaternion
	*/
	float Quaternion::magnitude() {
		return sqrt(w * w + i * i + j * j + k * k);
	}

	/*
	renvoie la norme au carre du quaternion
	*/
	float Quaternion::squareMagnitude() {
		return w * w + i * i + j * j + k * k;
	}

	/*
	 tourne le quaternion avec le vecteur donne en argument
	*/
	void Quaternion::rotateByVector(const Vector3D& vect) {

		Quaternion q = Quaternion(0, vect.x, vect.y, vect.z);
		this->hamiltonianProduct(q);

		data[0] = w;
		data[1] = i;
		data[2] = j;
		data[3] = k;
	}

	void Quaternion::printQuaternion() {
		std::cout << "w= " << w << " i= " << i << " j = " << j << " k = " << k << std::endl;
	}

	/*
	ajoute le vecteur donne en argument au quaternion, multiplie par le scalaire en argument
	*/
	void Quaternion::addScaledVector(const Vector3D& vect, float scale) {

		Quaternion q = Quaternion(0, vect.x * scale, vect.y * scale, vect.z * scale);
		q.hamiltonianProduct(*this);

		w += q.w * ((float)0.5);
		i += q.i * ((float)0.5);
		j += q.j * ((float)0.5);
		k += q.k * ((float)0.5);

		data[0] = w;
		data[1] = i;
		data[2] = j;
		data[3] = k;
	}

	void Quaternion::Write(serialization::OutputMemoryStream& stream) {
		stream.Write(w);
		stream.Write(i);
		stream.Write(j);
		stream.Write(k);
	}
	void Quaternion::Read(serialization::InputMemoryStream& stream) {
		w = stream.Read<float>();
		i = stream.Read<float>();
		j = stream.Read<float>();
		k = stream.Read<float>();
	}
	void Quaternion::OnInspectorGUI() {
		if (ImGui::CollapsingHeader("Rotation")) {
			ImGui::DragFloat4("Local Rotation vector", &w, 0.01f);
		}
	}
}