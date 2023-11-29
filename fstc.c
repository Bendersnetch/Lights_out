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

void playGame(Grid *grid) {
    int gameOver = 0;
    int input;

    do {
        clear();
        printGrid(grid);
        mvprintw(grid->cursorRow, grid->cursorCol * 2, "\u2588"); // Utilisation de l'Unicode pour un bloc plein
        refresh();

        input = getch();

        switch (input) {
            case KEY_UP:
                grid->cursorRow = (grid->cursorRow > 0) ? grid->cursorRow - 1 : 0;
                break;
            case KEY_DOWN:
                grid->cursorRow = (grid->cursorRow < grid->rows - 1) ? grid->cursorRow + 1 : grid->rows - 1;
                break;
            case KEY_LEFT:
                grid->cursorCol = (grid->cursorCol > 0) ? grid->cursorCol - 1 : 0;
                break;
            case KEY_RIGHT:
                grid->cursorCol = (grid->cursorCol < grid->cols - 1) ? grid->cursorCol + 1 : grid->cols - 1;
                break;
            case 10: // Enter key
                toggleCell(grid, grid->cursorRow, grid->cursorCol);
                gameOver = isGameOver(grid);
                autoSaveGame(grid, "autosave.txt"); // Sauvegarde automatique pendant le jeu
                break;
            case 27: // Escape key
                return; // Revenir au menu
            default:
                break;
        }

    } while (!gameOver);

    printw("Congratulations! You've turned off all the lights.\n");
    refresh();
    getch(); // Attendre que l'utilisateur appuie sur une touche pour continuer
}
void initializeGrid(Grid *grid) {
    // Initialise la grille avec des lumières allumées ou éteintes de manière aléatoire
    grid->rows = MIN_SIZE + rand() % (MAX_SIZE - MIN_SIZE + 1);
    grid->cols = MIN_SIZE + rand() % (MAX_SIZE - MIN_SIZE + 1);

    for (int i = 0; i < grid->rows; ++i) {
        for (int j = 0; j < grid->cols; ++j) {
            grid->lights[i][j] = rand() % 2; // 0 pour éteint, 1 pour allumé
        }
    }

    // Initialise le curseur au coin supérieur gauche
    grid->cursorRow = 0;
    grid->cursorCol = 0;
}

void printGrid(Grid *grid) {
    for (int i = 0; i < grid->rows; ++i) {
        for (int j = 0; j < grid->cols; ++j) {
            printw("[%c]", (grid->lights[i][j] == 1) ? '*' : ' ');
        }
        printw("\n");
    }
}

void toggleCell(Grid *grid, int row, int col) {
    // Change l'état de la cellule et de ses voisins
    grid->lights[row][col] = 1 - grid->lights[row][col]; // Inversion de 0 à 1 et de 1 à 0

    if (row > 0) {
        grid->lights[row - 1][col] = 1 - grid->lights[row - 1][col];
    }

    if (row < grid->rows - 1) {
        grid->lights[row + 1][col] = 1 - grid->lights[row + 1][col];
    }

    if (col > 0) {
        grid->lights[row][col - 1] = 1 - grid->lights[row][col - 1];
    }

    if (col < grid->cols - 1) {
        grid->lights[row][col + 1] = 1 - grid->lights[row][col + 1];
    }
}

int isGameOver(Grid *grid) {
    // Vérifie si toutes les lumières sont éteintes
    for (int i = 0; i < grid->rows; ++i) {
        for (int j = 0; j < grid->cols; ++j) {
            if (grid->lights[i][j] == 1) {
                return 0; // Le jeu n'est pas terminé
            }
        }
    }

    return 1; // Le jeu est terminé
}

void saveGame(Grid *grid, char *filename) {
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "%d %d\n", grid->rows, grid->cols);
        for (int i = 0; i < grid->rows; ++i) {
            for (int j = 0; j < grid->cols; ++j) {
                fprintf(file, "%d ", grid->lights[i][j]);
            }
            fprintf(file, "\n");
        }
        fclose(file);
        printw("Game saved successfully!\n");
    } else {
        printw("Error saving game.\n");
    }
    refresh();
}

void autoSaveGame(Grid *grid, char *filename) {
    // Sauvegarde automatique de la partie en cours
    saveGame(grid, filename);
}

void loadGame(Grid *grid, char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fscanf(file, "%d %d", &(grid->rows), &(grid->cols));
        for (int i = 0; i < grid->rows; ++i) {
            for (int j = 0; j < grid->cols; ++j) {
                fscanf(file, "%d", &(grid->lights[i][j]));
            }
        }
        fclose(file);
        printw("Game loaded successfully!\n");
    } else {
        printw("Error loading game.\n");
    }
    refresh();
}

void loadAutoSavedGame(Grid *grid, char *filename) {
    // Charger automatiquement la partie précédemment sauvegardée
    loadGame(grid, filename);
}

void drawMenu(int highlight, char *choices[], int numChoices) {
    int x, y, i;

    x = (COLS - 20) / 2; // Centrer horizontalement
    y = (LINES - numChoices) / 2; // Centrer verticalement

    for (i = 0; i < numChoices; ++i) {
        if (highlight == i) {
            attron(A_REVERSE);
            mvprintw(y, x, "  > %s", choices[i]); // Ajout de l'espace avant l'option sélectionnée
            attroff(A_REVERSE);
        } else {
            mvprintw(y, x, "   %s", choices[i]); // Ajout de l'espace avant les autres options
        }
        ++y;
    }

    refresh();
}

int showMenu(char *title, char *choices[], int numChoices) {
    int highlight = 0;
    int choice;
    int c;

    do {
        clear();

        attron(A_BOLD);  // Activer le texte en gras
        // Dessiner la bordure supérieure du titre
        mvhline((LINES - numChoices) / 2 - 3, (COLS - strlen(title)) / 2 - 2, '-', strlen(title) + 4);
        // Dessiner le titre avec la bordure latérale
        mvprintw((LINES - numChoices) / 2 - 2, (COLS - strlen(title)) / 2 - 2, "| %s |", title);
        // Dessiner la bordure inférieure du titre
        mvhline((LINES - numChoices) / 2 - 1, (COLS - strlen(title)) / 2 - 2, '-', strlen(title) + 4);        
        attroff(A_BOLD);  // desactive le texte en gras
        
        drawMenu(highlight, choices, numChoices);

        c = getch();
        switch (c) {
            case KEY_UP:
                if (highlight > 0) {
                    highlight--;
                }
                break;
            case KEY_DOWN:
                if (highlight < numChoices - 1) {
                    highlight++;
                }
                break;
            case 10: // Enter key
                choice = highlight;
                break;
            default:
                break;
        }

    } while (c != 10);

    return choice;
}

void showTitleScreen() {
    clear();
    mvprintw(LINES / 2, (COLS - 10) / 2, "Lights Out");
    refresh();
    getch();
}