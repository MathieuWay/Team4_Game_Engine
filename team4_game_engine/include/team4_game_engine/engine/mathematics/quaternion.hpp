#pragma once
namespace team4_game_engine::serialization {
	class OutputMemoryStream;
	class InputMemoryStream;
}
namespace team4_game_engine::engine::mathematics {
	class Vector3D;
	class Quaternion {

	public:
		union {
			struct {
				/*
			contient la valeur reel
			*/
				float w;

				/*
				contient la premier valeur imaginaire
				*/
				float i;

				/*
				contient la deuxieme valeur imaginaire
				*/
				float j;

				/*
				contient la troisieme valeur imaginaire
				*/
				float k;
			};

			/*
			contient le quaternion dans une array
			*/
			float data[4];

		};

		/*
		creer un quaternion avec des valeurs à 0
		*/
		Quaternion();

		/*
		creer un quaternion avec les valeurs passe en argument
		*/
		Quaternion(float w, float i, float j, float k);

		~Quaternion();

		/*
		normalise le quaternion
		*/
		void normalize();

		/*
		fait le produit hamiltonien du quaternion et du quaternion passe en argument
		*/
		void hamiltonianProduct(Quaternion q);

		/*
		inverse le quaternion en argument
		*/
		void invert();

		/*
		renvoie la norme du quaternion
		*/
		float magnitude();

		/*
		renvoie la norme au carre du quaternion
		*/
		float squareMagnitude();

		/*
		 tourne le quaternion avec le vecteur donne en argument
		*/
		void rotateByVector(const Vector3D& vect);

		/*
		print the Vector
		*/
		void printQuaternion();

		/*
		ajoute le vecteur donne en argument au quaternion, multiplie par le scalaire en argument
		*/
		void addScaledVector(const Vector3D& vector, float scale);

		void Write(serialization::OutputMemoryStream& stream);

		void Read(serialization::InputMemoryStream& stream);

		void OnInspectorGUI();
	};
}