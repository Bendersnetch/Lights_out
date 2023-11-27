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
void showTitleScreen();

int main() {
    srand(time(NULL));

    initscr(); // Initialiser la bibliothèque ncurses
    keypad(stdscr, TRUE);

    int choice;
    char filename[50];

    showTitleScreen();

    char *menuChoices[] = {
        "Nouvelle Partie",
        "Charger une partie",
        "Quitter"
    };

    int numMenuChoices = sizeof(menuChoices) / sizeof(menuChoices[0]);

    do {
        clear();
        choice = showMenu("Menu Principal", menuChoices, numMenuChoices);

        switch (choice) {
            case 0: // Start New Game
            {
                Grid gameGrid;
                initializeGrid(&gameGrid);

                int gameOver = 0;
                int input;

                do {
                    clear();
                    printGrid(&gameGrid);
                    mvprintw(gameGrid.cursorRow, gameGrid.cursorCol * 2, "[*]");
                    refresh();

                    input = getch();

                    switch (input) {
                        case KEY_UP:
                            gameGrid.cursorRow = (gameGrid.cursorRow > 0) ? gameGrid.cursorRow - 1 : 0;
                            break;
                        case KEY_DOWN:
                            gameGrid.cursorRow = (gameGrid.cursorRow < gameGrid.rows - 1) ? gameGrid.cursorRow + 1 : gameGrid.rows - 1;
                            break;
                        case KEY_LEFT:
                            gameGrid.cursorCol = (gameGrid.cursorCol > 0) ? gameGrid.cursorCol - 1 : 0;
                            break;
                        case KEY_RIGHT:
                            gameGrid.cursorCol = (gameGrid.cursorCol < gameGrid.cols - 1) ? gameGrid.cursorCol + 1 : gameGrid.cols - 1;
                            break;
                        case 10: // Enter key
                            toggleCell(&gameGrid, gameGrid.cursorRow, gameGrid.cursorCol);
                            gameOver = isGameOver(&gameGrid);
                            break;
                        default:
                            break;
                    }

                } while (!gameOver);

                printw("Félicitation! Tu a allumer tout les lumières.\n");
                refresh();
                getch(); // Attendez que l'utilisateur appuie sur une touche pour continuer
            }
            break;

            case 1: // Load Game
                printw("Entre le nom du fichier a charger ");
                refresh();
                getstr(filename);
                // Load game function
                break;

            case 2: // Quit
                break;

            default:
                printw("Choix invalide. Essaye encore\n");
                refresh();
        }

    } while (choice != numMenuChoices - 1);

    endwin(); // Terminer l'utilisation de la bibliothèque ncurses

    return 0;
}


