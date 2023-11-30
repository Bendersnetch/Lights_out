#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <ncurses.h>
#include <unistd.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <strings.h>
#include <locale.h>
#include "protos.h"

// Fonction pour afficher un écran titre centré
// Fonction pour afficher un écran titre centré
void showTitleScreen();

// Fonction pour le gameplay
void playGame(Grid *grid);

int main() {
    srand(time(NULL));

    initscr(); // Initialiser la bibliothèque ncurses
    start_color(); // Activer la couleur
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    setlocale(LC_ALL, "");
    keypad(stdscr, TRUE);

    int choice;
    char filename[50];

    showTitleScreen();

    char *menuChoices[] = {
        "Nouvelle partie",
        "Charger partie",
        "Continuer Partie", 
        "Sauvegarder Partie",
        "Quitter"
    };

    int numMenuChoices = sizeof(menuChoices) / sizeof(menuChoices[0]);

    Grid gameGrid;
    loadAutoSavedGame(&gameGrid, "autosave.txt"); // Charger la sauvegarde automatique

    do {
        clear();

         // Affiche "Continuer Partie" seulement s'il y a un fichier de sauvegarde
        if (hasSaveFile("autosave.txt")) {
            menuChoices[2] = "Continuer Partie";
            menuChoices[3] = "Sauvegarder Partie";
        } else {
            menuChoices[2] = "";  // Option vide si pas de sauvegarde
            menuChoices[3] = "";
        }
        
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

            case 2:  // Continue Game
                if (strlen(menuChoices[2]) > 0) {
                    playGame(&gameGrid);
                    break;
                }

            case 3: // Save Game
                if (hasSaveFile("autosave.txt")){
                    printw("Entrer le nom du fichier a sauvegarder ");
                    refresh();
                    getstr(filename);
                    saveGame(&gameGrid, filename);
                    break;
                }

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