#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <strings.h>
#include "protos.h"

// Fonction pour afficher un écran titre centré
// Fonction pour afficher un écran titre centré
void showTitleScreen();

// Fonction pour le gameplay
void playGame(Grid *grid);

int main() {
    srand(time(NULL));

    initscr(); // Initialiser la bibliothèque ncurses
    keypad(stdscr, TRUE);

    int choice;
    char filename[50];

    showTitleScreen();

    char *menuChoices[] = {
        "Nouvelle partie",
        "Charger partie",
        "Continuer la partie", 
        "Sauvegarder la partie",
        "Quitter"
    };

    int numMenuChoices = sizeof(menuChoices) / sizeof(menuChoices[0]);

    Grid gameGrid;
    loadAutoSavedGame(&gameGrid, "autosave.txt"); // Charger la sauvegarde automatique

    do {
        clear();
        choice = showMenu("Menu principal", menuChoices, numMenuChoices);

        switch (choice) {
            case 0: // Start New Game
                initializeGrid(&gameGrid);
                playGame(&gameGrid);
                break;

            case 1: // Load Game
                printw("Entrer le nom du fichier a charger");
                refresh();
                getstr(filename);
                loadGame(&gameGrid, filename);
                playGame(&gameGrid);
                break;

            case 2: // Continue Game
                playGame(&gameGrid);
                break;

            case 3: // Save Game
                printw("Entrer le nom du fichier a sauvegarder ");
                refresh();
                getstr(filename);
                saveGame(&gameGrid, filename);
                break;

            case 4: // Quit
                break;

            default:
                printw("Choix invalide, ressaye s'il te plait\n");
                refresh();
        }

    } while (choice != numMenuChoices - 1);

    autoSaveGame(&gameGrid, "autosave.txt"); // Sauvegarde automatique avant la sortie

    endwin(); // Terminer l'utilisation de la bibliothèque ncurses

    return 0;
}