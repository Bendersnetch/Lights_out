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
    curs_set(0);
    start_color(); // Activate colors
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    setlocale(LC_ALL, "");
    keypad(stdscr, TRUE);

    int choice;
    char filename[50];

    showTitleScreen();

    char *menuChoices[] = {
        "New game",
        "New custom game",
        "Load game",
        "Continue game",
        "Save Game",
        "Leave"
    };

    int numMenuChoices = sizeof(menuChoices) / sizeof(menuChoices[0]);

     // Initialisation de la structure Grid avec malloc
    Grid *grid = (Grid *)malloc(sizeof(Grid));
    if (grid == NULL) {
        perror("Erreur d'allocation mémoire");
        endwin();
        return 1;
    }
    loadGame(grid, "autosave.txt"); // Charger la sauvegarde automatique

    do {
        clear();

         //Only show "continue" if there's a save file
        if (hasSaveFile("autosave.txt")) {
            menuChoices[3] = "Continue game";
            menuChoices[4] = "Save Game";
        } else {
            menuChoices[3] = "";  // Empty option if no file
            menuChoices[4] = "";
        }

        choice = showMenu("Main menu", menuChoices, numMenuChoices);
        switch (choice) {
            case 0: // Start New Game
//                initializeGrid(grid);
//                playGame(grid);
                chooseSize(grid);
                break;

            case 1: // Create a custom sized game
                initializeCustomGrid(grid);
                playGame(grid);
                break;

            case 2: // Load Game
                curs_set(2);
                printw("Enter the file name ");
                refresh();
                getstr(filename);
                curs_set(0);
                loadGame(grid, filename);
                playGame(grid);
                break;

            case 3:  // Continue Game
                if (strlen(menuChoices[2]) > 0) {
                    playGame(grid);
                    break;
                }

            case 4: // Save Game
                if (hasSaveFile("autosave.txt")){
                    curs_set(2);
                    printw("Enter the file name to save ");
                    refresh();
                    getstr(filename);
                    curs_set(0);
                    saveGame(grid, filename);
                    break;
                }

            case 5: // Quit
                break;

            default:
                printw("Invalid choice, please retry.\n");
                refresh();
        }

    } while (choice != numMenuChoices - 1);

    // Libération de la mémoire allouée dynamiquement
    for (int i = 0; i < grid->rows; ++i) {
        free(grid->lights[i]);
    }
    free(grid->lights);

    free(grid);

    endwin(); // Stop curses library utilization

    return 0;
}
