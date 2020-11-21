#pragma once
#include "team4_game_engine/components/rigidbody.h"
#include "team4_game_engine/physics/force_generator.h"
#include <vector>

namespace team4_game_engine::physics {
	//Création d'un registre contenant toutes forces en action ainsi que la particule sur laquelle chaque force s'applique
	class ForceRegistry
	{
	protected:

		//Elément contenant une force et la particule sur laquelle elle s'applique
		struct Registration
		{
			Particule* particle;
			ParticleForceGenerator* fg;
		};

		//Redéfinition de type pour ne pas à avoir à utiliser un type à rallonge
		typedef std::vector<Registration> Registry;

		//Création de la liste de registration
		Registry registrations;

	public:

		//Cette fonction permet d'ajouter une force à la fin du registre, on peut mettre deux fois la même force sur la même particule (à voir plus tard si pertinent)
		void Add(Particule* particle, ParticleForceGenerator* fg)
		{
			Registration newRegitration = {
											newRegitration.particle = particle,
											newRegitration.fg = fg
			};

			registrations.push_back(newRegitration);
		}


		//Cette fonction permet d'enlever une force du registre, si cette force n'est pas dans le registre cela ne fait rien
		void Remove(Particule* particle, ParticleForceGenerator* fg)
		{
			int i = 0;
			while (registrations[i].particle != particle || registrations[i].fg != fg) i++;

			if (i == registrations.size()) return;

			registrations.erase(registrations.begin() + i);
		}

		void RemoveAll(Particule* particle)
		{
			for (int i = 0; i < registrations.size(); i++)
			{
				if (registrations[i].particle == particle) {
					Particule* secondpart = registrations[i].fg->secondPart;
					registrations.erase(registrations.begin() + i);
					i--;
					if (secondpart != nullptr) {
						int j = 0;
						while (j < registrations.size())
						{
							if (particle == registrations[j].fg->secondPart &&
								secondpart == registrations[j].particle) {
								registrations.erase(registrations.begin() + j);
								i--;
								break;
							}
							j++;
						}
					}
				}
			}
		}


		//Cette fonction vide le registre de toutes ses forces
		void Clear()
		{
			registrations.clear();
		}


		//Cette fonction permet de mettre à jour les forces appliquées aux particules et cela pour toutes les forces du registre
		void UpdateForces(float deltaTime)
		{
			for (Registration r : registrations)
			{
				r.fg->UpdateForce(r.particle, deltaTime);
			}
		}
	};
}