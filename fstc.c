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

void initializeGrid(Grid *grid) {
    grid->rows = 4;
    grid->cols = 4;
    grid->cursorRow = 0;
    grid->cursorCol = 0;

    for (int i = 0; i < grid->rows; ++i) {
        for (int j = 0; j < grid->cols; ++j) {
            grid->lights[i][j] = 0; // All lights off initially
        }
    }
}

void printGrid(Grid *grid) {
    printw("\nCurrent Grid:\n");
    for (int i = 0; i < grid->rows; ++i) {
        for (int j = 0; j < grid->cols; ++j) {
            if (i == grid->cursorRow && j == grid->cursorCol) {
                attron(A_REVERSE);
            }
            printw("%c ", (grid->lights[i][j] == 1) ? 'X' : '.');
            if (i == grid->cursorRow && j == grid->cursorCol) {
                attroff(A_REVERSE);
            }
        }
        printw("\n");
    }
}

void toggleCell(Grid *grid, int row, int col) {
    if (row >= 0 && row < grid->rows && col >= 0 && col < grid->cols) {
        grid->lights[row][col] = !grid->lights[row][col];

        // Toggle neighbors
        if (row - 1 >= 0) grid->lights[row - 1][col] = !grid->lights[row - 1][col];
        if (row + 1 < grid->rows) grid->lights[row + 1][col] = !grid->lights[row + 1][col];
        if (col - 1 >= 0) grid->lights[row][col - 1] = !grid->lights[row][col - 1];
        if (col + 1 < grid->cols) grid->lights[row][col + 1] = !grid->lights[row][col + 1];
    }
}

int isGameOver(Grid *grid) {
    // Check if all lights are off
    for (int i = 0; i < grid->rows; ++i) {
        for (int j = 0; j < grid->cols; ++j) {
            if (grid->lights[i][j] == 1) {
                return 0; // Game is not over
            }
        }
    }
    return 1; // Game is over
}

void drawMenu(int highlight, char *choices[], int numChoices) {
    int x, y, i;

    x = (COLS - 20) / 2; // Centrer horizontalement
    y = (LINES - numChoices) / 2; // Centrer verticalement

    for (i = 0; i < numChoices; ++i) {
        if (highlight == i) {
            attron(A_REVERSE);
            mvprintw(y, x, "> %s", choices[i]);
            attroff(A_REVERSE);
        } else {
            mvprintw(y, x, "  %s", choices[i]);
        }
        ++y;
    }

    refresh();
}

int showMenu(char *title, char *choices[], int numChoices) {
    int highlight = 0;
    int choice = 0;
    int c;

    do {
        clear();
        mvprintw(0, (COLS - strlen(title)) / 2, "%s", title);
        drawMenu(highlight, choices, numChoices);

        c = getch();
        switch (c) {
            case KEY_UP:
                highlight = (highlight == 0) ? numChoices - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == numChoices - 1) ? 0 : highlight + 1;
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
    char *title = "Lights Out";
    mvprintw(LINES / 2, (COLS - strlen(title)) / 2, "%s", title);
    refresh();
    getch(); // Attendre que l'utilisateur appuie sur une touche pour continuer
}