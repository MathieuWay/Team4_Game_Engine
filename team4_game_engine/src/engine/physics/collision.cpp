#include <team4_game_engine/engine/physics/collision.hpp>
#include <team4_game_engine/engine/engine.hpp>
#include <team4_game_engine/engine/world.hpp>

namespace team4_game_engine::physics {
	Collision::Collision(entt::entity _entities[2], RigidBody* _rigidBodies[2], float res, float pen, Vector3D contNorm) : restitution(res), penetration(pen), contactNormal(contNorm) {
		entities[0] = _entities[0];
		entities[1] = _entities[1];
		rigidBodies[0] = _rigidBodies[0];
		rigidBodies[1] = _rigidBodies[1];
	}

	void Collision::resolve(float deltatime) {
		resolveInterpenetration(deltatime);
		resolveVelocity(deltatime);
	}

	float Collision::calculateSeparateVelocity() const {
		// on recupere la vitesse de la premiere particule
		Vector3D rVelocity = rigidBodies[0]->linearVelocity;

		if (rigidBodies[1]) { // si la collision à lieu avec une autre particule on soustrait la vitesse de la premiere particule par la deuxieme 
			rVelocity = rVelocity.subVector(rigidBodies[1]->linearVelocity);
		}
		// on renvoi le produit scalaire de la vitesse calulee avec la Normal au point de contact
		return rVelocity.scalarProduct(contactNormal);
	}

	void Collision::resolveVelocity(float deltatime) {

		// calcul la vitesse dans la direction du contact
		float separatingVelocity = calculateSeparateVelocity();

		if (separatingVelocity > 0) {
			return;
		}
		// calcul la nouvelle vitesse de separation
		float newSepVelocity = -separatingVelocity * restitution;
		Vector3D accCausedVelocity = rigidBodies[0]->acceleration;
		if (rigidBodies[1])
		{
			accCausedVelocity = accCausedVelocity.subVector(rigidBodies[1]->acceleration);
		}
		float accCausedSepVelocity = accCausedVelocity.scalarProduct(contactNormal) * deltatime;

		if (accCausedSepVelocity < 0)
		{
			newSepVelocity += restitution * accCausedSepVelocity;

			if (newSepVelocity < 0) newSepVelocity = 0;
		}

		float deltaVelocity = newSepVelocity - separatingVelocity;

		float sumInverseMass = rigidBodies[0]->inverseMass;
		if (rigidBodies[1]) { // si la colision à lieu avec une autre particule on ajoute son inverse de la masse 
			sumInverseMass += rigidBodies[1]->inverseMass;
		}

		if (sumInverseMass <= 0) { // si la masse des particules est infini, l'impulsion est inutile
			return;
		}

		// calcul l'impulsion à appliquer aux particules 
		Vector3D impulseMass = contactNormal.scalarMultiplication(deltaVelocity / sumInverseMass);
		Vector3D impulseP0 = rigidBodies[0]->linearVelocity.sumVector(impulseMass.scalarMultiplication(rigidBodies[0]->inverseMass));
		//std::cout << "p0 :" << std::endl;
		//impulseP0.printVector();
		// applique l'impulsion aux particules proportionnelement à l'inverse de leur masse
		if(!rigidBodies[0]->isKinematic)
			rigidBodies[0]->linearVelocity = impulseP0;

		if (rigidBodies[1] != NULL) {
			Vector3D impulseP1 = rigidBodies[1]->linearVelocity.sumVector(impulseMass.scalarMultiplication(-rigidBodies[1]->inverseMass));
			//std::cout << "p1 :" << std::endl;
			//impulseP1.printVector();
			if (!rigidBodies[1]->isKinematic)
				rigidBodies[1]->linearVelocity = impulseP1;
		}
	}

	void Collision::resolveInterpenetration(float deltatime) {

		// si il n'y a pas de penetration on passe l'etape
		if (penetration <= 0) return;

		float sumInverseMass = rigidBodies[0]->inverseMass;
		if (rigidBodies[1]) { // si la colision à lieu avec une autre particule on ajoute son inverse de la masse 
			sumInverseMass += rigidBodies[1]->inverseMass;
		}

		if (sumInverseMass <= 0) { // si la masse des particules est infini, l'impulsion est inutile
			return;
		}
		// le deplacement des particules depend de l'inverse de leur masse 
		// on calcul donc la penetration par unite d'inverse de masse
		Vector3D moveMass = contactNormal.scalarMultiplication(penetration / sumInverseMass);

		// on calcul le mouvement realise pas les particules en fonction de l'inverse de leur masse

		rigidbodyMove[0] = moveMass.scalarMultiplication(rigidBodies[0]->inverseMass);

		if (rigidBodies[1]) { // si la colision à lieu avec une autre particule on calcul le deplacement de l'autre particule 
			rigidbodyMove[1] = moveMass.scalarMultiplication(-rigidBodies[1]->inverseMass);
		}
		else {
			rigidbodyMove[1] = Vector3D();
		}

		//float sumInverseMass = particlesContact[0]->GetMass();
		//if (particlesContact[1])
		//	sumInverseMass += particlesContact[1]->GetMass();
		//sumInverseMass = 1 / sumInverseMass;

		//Vector3D moveMass = contactNormal.scalarMultiplication(penetration).scalarMultiplication(sumInverseMass);

		////// on calcul le mouvement realise pas les particules en fonction de l'inverse de leur masse
		//
		//particleMove[0] = moveMass.scalarMultiplication(particlesContact[0]->GetMass());
		//
		//if (particlesContact[1]) { // si la colision à lieu avec une autre particule on calcul le deplacement de l'autre particule 
		//	particleMove[1] = moveMass.scalarMultiplication(-particlesContact[1]->GetMass());
		//}
		//else {
		//	particleMove[1] = Vector3D();
		//}
		//if (particlesContact[1]) {
		//	//Particule
		//	Vector3D particleMoveP0 = particlesContact[0]->GetPosition().sumVector(contactNormal.scalarMultiplication(penetration).scalarMultiplication(0.5f));
		//	particlesContact[0]->SetPosition(particleMoveP0);

		//	Vector3D particleMoveP1 = particlesContact[1]->GetPosition().sumVector(contactNormal.scalarMultiplication(penetration).scalarMultiplication(0.5f));
		//	particlesContact[1]->SetPosition(particleMoveP1);
		//}
		//else {
		//	//SOL
		//	Vector3D particleMoveP0 = particlesContact[0]->GetPosition().sumVector(contactNormal.scalarMultiplication(penetration));
		//	particlesContact[0]->SetPosition(particleMoveP0);
		//}
		auto& reg = engine::Engine::Instance().GetWorld().lock()->Registry();
		if (!rigidBodies[0]->isKinematic) {
			auto& posP0 = reg.get<Position>(entities[0]);
			// on applique la resolution aux particules
			Vector3D particleMoveP0 = posP0.local.sumVector(rigidbodyMove[0]);
			posP0.SetPosition(particleMoveP0.x, particleMoveP0.y, particleMoveP0.z);
		}
		if (rigidBodies[1]) {
			if (!rigidBodies[1]->isKinematic) {
				auto& posP1 = reg.get<Position>(entities[1]);
				Vector3D particleMoveP1 = posP1.local.sumVector(rigidbodyMove[1]);
				posP1.SetPosition(particleMoveP1.x, particleMoveP1.y, particleMoveP1.z);
			}
		}

	}

}