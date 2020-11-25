- Avoir Visual Studio 2019 + extension visual studio C++
- Avoir CMake Version 3.10 minimum
- Compiler le projet avec CMake
- Ouvrir dans le projet avec votre IDE
- Compiler les sources du projet
- Lancer l'éxécutable compiler dans le dossier "./examples/basic_usage"





Une fois dans la fenêtre, Il y a trois menu vous permettant de gérer le moteur.

En haut au millieu se trouve la "Physics Winow":

	- Il est possible de choisir un DeltaTime fixe (et ainsi le nombre d'étapes physiques par secondes).
	- Il est possible de stopper la simulation et passer les étapes physiques manuellement.
	- Il est possible d'activer les collisions entre les corps rigides.



Sur la gauche se situe la hiérarchie de la scène.

- Le cube situé en haut vous permet de déplacer la caméra autour de la scène.
- Vous pouvez sélectionner les objets qui ont été créer dans la scène pour pouvoir déplacer ces derniers manuellement.
- Le bouton "Clean Scene" permet de supprimer tout les objets présent dans la scène.
- Le bouton "Add entity" permet d'ajouter un objet dans la scène.
- Un clic droit vous permet aussi de créer un objet ou vous voulez dans la hiérachie.




En haut à droite se situe la fenêtre d'inspection.
Toutes les données de l'objet y sont regroupé par catégorie

- Dans les catégories Position / Rotation / Scale / Color:
	- Les données de position, rotation, taille et de couleur de l'objet sont accéssibles et modifiables.


- Dans Les catégories Box / Sphere Collider / RigidBody:
	- Les données de physique (mass, drag, gravité, restitution, etc...) et de collision (sa taille de boite de collision) sont accéssibles et modifiables.

- Dans La catégorie Debug Force:
	-Vous pouvez ajouter une force en donnant son point d'application et sa direction puis en cliquant sur "Add Force".

- Dans les catégorie Model:
	- Cette catégorie n'est pas utilisée pour le moment.