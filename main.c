#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <curses.h>
#include <unistd.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <strings.h>
#include <locale.h>
#include "protos.h"

//Centered title screen
void showTitleScreen();

// Fonction pour le gameplay
void playGame(Grid *grid);

int main() {
    srand(time(NULL));

    initscr(); // Initialize curses library
    start_color(); // Activate colors
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    setlocale(LC_ALL, "");
    keypad(stdscr, TRUE);

    int choice;
    char filename[50];

    showTitleScreen();

    char *menuChoices[] = {
        "Nouvelle partie",
        "Nouvelle partie a taille choisie",
        "Charger partie",
        "Continuer Partie",
        "Sauvegarder Partie",
        "Quitter"
    };

    int numMenuChoices = sizeof(menuChoices) / sizeof(menuChoices[0]);

    Grid gameGrid;
    loadAutoSavedGame(&gameGrid, "autosave.txt"); // load a saved game

    do {
        clear();

         //Only show "continue" if there's a save file
        if (hasSaveFile("autosave.txt")) {
            menuChoices[3] = "Continuer Partie";
            menuChoices[4] = "Sauvegarder Partie";
        } else {
            menuChoices[3] = "";  // Empty option if no file
            menuChoices[4] = "";
        }

        choice = showMenu("Menu principal", menuChoices, numMenuChoices);

        switch (choice) {
            case 0: // Start New Game
                initializeGrid(&gameGrid);
                playGame(&gameGrid);
                break;

            case 1: // Create a custom sized game
                initializeCustomGrid(&gameGrid);
                playGame(&gameGrid);
                break;

            case 2: // Load Game
                printw("Entrer le nom du fichier a charger ");
                refresh();
                getstr(filename);
                loadGame(&gameGrid, filename);
                playGame(&gameGrid);
                break;

            case 3:  // Continue Game
                if (strlen(menuChoices[2]) > 0) {
                    playGame(&gameGrid);
                    break;
                }

            case 4: // Save Game
                if (hasSaveFile("autosave.txt")){
                    printw("Entrer le nom du fichier a sauvegarder ");
                    refresh();
                    getstr(filename);
                    saveGame(&gameGrid, filename);
                    break;
                }

            case 5: // Quit
                break;

            default:
                printw("Choix invalide, ressaye s'il te plait\n");
                refresh();
        }

    } while (choice != numMenuChoices - 1);

    autoSaveGame(&gameGrid, "autosave.txt"); // Autoload when exiting the game

    endwin(); // Stop curses library utilization

    return 0;
}
