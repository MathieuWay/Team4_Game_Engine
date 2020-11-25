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
- Vous pouvez sélectionner les particules des objets qui ont été créer dans la scène pour pouvoir déplacer ces derniers manuellement.
- Le bouton "Clean Scene" permet de supprimer tout les objets présent dans la scène.
- Le bouton "Add entity" permet d'ajouter un objet dans la scène.




En haut à droite se situe la fenêtre d'inspection.
Toutes les données de l'objet y sont regroupé par catégorie

- Dans les catégorie Position / Rotation / Scale / Color
	- Les données de position, rotation, taille et de couleur de l'objet sont accéssible et modifiable


- Dans Les catégorie Box / Sphere Collider / RigidBody:
	- Les données de physique (mass, drag, gravité, restitution, etc...) et de collision (sa taille de boite de collision)


- Dans les catégorie Model:
	- Cette catégorie n'est pas utilisé pour le moment





