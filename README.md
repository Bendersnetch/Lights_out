# Lights_out
SAE, BUT informatique première année, S1

## Pincipe général du jeux
Lights Out est un jeu électronique vendu par Tiger Electronics en 1951. Il y a une grille de 5 x 5
lumières. Lorsque le jeu démarre, un nombre aléatoire de lumières sont allumées. En appuyant
sur une lumière on change son état (si elle est allumée, elle s’éteint, et si elle est éteinte, elle
s’allume) ainsi que l’état de ses 4 voisins. Le but du jeu est de trouver la combinaison de boutons
qui éteint le jeu entièrement en appuyant sur le moins de boutons possible.

## Objectif du jeu

L’objectiif de ce projet est de coder le jeu intitulé Lights Out et de permettre à une personne d’y
jouer. Il faut donc écrire un code qui :

- Propose en début de jeu un menu. Ce menu va permettre de configurer la taille de la
grille : le joueur peut déterminer la taille de la grille. Par défaut cette taille est fixée à
4x4.
- Représente une grille de dimension LxC composées de cases qui sont soit éteintes soit
allumées
-  Place aléatoirement un certain nombre de lumières allumées au début d’une partie. 
- Permet de stocker une partie en cours dans un fichier texte et de pouvoir la recharger
si besoin

## Contraintes
 - Le programme doit être dev en C
 - Le programme doit s'executer en mode console
 - il doit tourner sous windows, mac et linux

````
Fonction    : nom de la fonction/procedure
Auteur      : nom de la ou des auteur des fonctions ou procdeures
Param       : signiification des paramètres
Traitement  : résumé succint de son traitement
Retour      : signification des valeurs retournées
````

## Cahier des charges
- Créer un menu, avec differents choix
    - grille par défaut (4x4)
    - grille 5x5, 6x6, 7x7...
    - Grille personalisée (LxC)
- Ajoute de la possibilité de charger une partie en cours
- Créer une grille (LxC) avec des lumières allumées, placer aléatoirement
    - Ajout d'un bouton pour enregistrer dans le repertoire courant la partie
        - conditions, on doit savoir quand la partie est terminée ou non
    - Permet à un joueur de sélectionner des cases pour changer leur état grace au clavier

## Repartition des taches (Tina, Hugo, Federico)
- Création du menu, et la gestion des grilles (Hugo)
- Création du Gamedesign, userInterface, IHM (Federico)
    - Grille
    - Bouton 
    - partie en cours (Hugo)
- Déplacement dans le jeux (clavier) (Tina)

# Décisions de Conception
### Structure du Projet
 Le projet est organisé en trois fichiers principaux : protos.h, fstc.c, et main.c.

- protos.h : Ce fichier d'en-tête contient la définition de la structure Grid représentant l'état du jeu, ainsi que les prototypes des fonctions associées au jeu et au menu.

- fstc.c : Dans ce fichier source, l'implémentation des fonctions définies dans protos.h est réalisée. Il contient également les fonctions spécifiques à l'affichage du jeu et du menu.

-  main.c : Ce fichier source contient la fonction principale (main) du programme. Il gère l'initialisation du jeu, l'affichage du menu principal et l'appel des fonctions appropriées en fonction des choix de l'utilisateur.

### Structure de Données
La principale structure de données utilisée est Grid, qui représente l'état de la grille du jeu. Elle contient des informations sur le nombre de lignes et de colonnes, l'état des lumières, ainsi que la position du curseur.

### Fonctions du Jeu
Les fonctions liées au jeu comprennent l'initialisation de la grille, l'affichage de la grille, la modification de l'état des cellules, la vérification de la fin de partie, ainsi que la sauvegarde et le chargement de parties.

### Fonctions du Menu
Les fonctions liées au menu sont responsables de l'affichage du menu, de la gestion de la navigation et de la récupération des choix de l'utilisateur.

### Choix d'Interface Utilisateur
L'utilisation de la bibliothèque ncurses nous permet d'avoir une interface utilisateur agreable (userfriendly). La gestion des menus, la navigation avec les touches fléchées, et l'affichage en mode console sont des exemples de la puissance de cet bibilotheques.
### Extensions Possibles
Le code est conçu de manière modulaire pour permettre des extensions futures. Des fonctionnalités telles que la gestion de différentes couleurs, la modification des règles du jeu, ou l'ajout de fonctionnalités de grille non rectangulaire peuvent être envisagées.
