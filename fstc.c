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
#include "protos.h"
#include <locale.h>
#include <stdbool.h>

#define MAX_SIZE 11
#define MIN_SIZE 4


/**
 * Represente le gameplay de la partit, LE JEUX EN GROS !!!
 * @author Hugo
 * @brief Gameplay du jeux
 * @param *grid --> Grid : accedes a la structure grid (cursorRow,cursorCol...)
*/
void playGame(Grid *grid) {
    int gameOver = 0;
    int input;

    do {
        clear();
        // Afficher le nom du jeu au-dessus de la grille
        mvprintw((LINES - grid->rows) / 2 - 5, (COLS - 10) / 2, "Lights Out");
        mvprintw((LINES - grid->rows) / 2 - 4, (COLS - 27) / 2, "You have already done %d moves", grid->moves);

        mvprintw((LINES - grid->rows) / 2 - 1, 3, "Instructions: ");
        mvprintw((LINES - grid->rows) / 2 + 1, 3, "Use the arrows to navigate");
        mvprintw((LINES - grid->rows) / 2 + 3, 3, "Press enter to change a light's state");
        mvprintw((LINES - grid->rows) / 2 + 5, 3, "Press esc to return t the menu");

        // Afficher les règles à droite de la grille
        mvprintw((LINES - grid->rows) / 2 - 1, COLS - 35, "Rules:");
        mvprintw((LINES - grid->rows) / 2 + 1, COLS - 35, "Turn off all lights to win");


        //affiche la grille de jeux
        printGrid(grid);

        printCursor(grid);

        input = getch();

        switch (input) {
            case KEY_UP:
                grid->cursorRow = (grid->cursorRow > 0) ? grid->cursorRow - 1 : 0;
                break;
            case KEY_DOWN:
                grid->cursorRow = (grid->cursorRow < grid->rows - 1) ? grid->cursorRow + 1 : grid->rows - 1;
                break;
            case KEY_LEFT:
                // Déplacer le curseur d'une case [ ] à une autre
                grid->cursorCol = (grid->cursorCol > 0) ? grid->cursorCol - 1 : 0;
                break;
            case KEY_RIGHT:
                // Déplacer le curseur d'une case [ ] à une autre
                grid->cursorCol = (grid->cursorCol < grid->cols - 1) ? grid->cursorCol + 1 : grid->cols - 1;
                break;
            case 10: // Enter key
                grid->moves++;
                toggleCell(grid, grid->cursorRow, grid->cursorCol);
                gameOver = isGameOver(grid);
                saveGame(grid, "autosave.txt"); //Sauvegarde automatique pendant le jeu
                break;
            case 27: // Escape key
                saveGame(grid, "autosave.txt");
                return; // Revenir au menu
            default:
                break;
        }

    } while (!gameOver);

    clear();
    printw("Congratulations! You've turned off all the lights in %d moves.\n", grid->moves);
    refresh();
    getch(); // Attendre que l'utilisateur appuie sur une touche pour continuer
}

void chooseSize (Grid *grid)
{
    int choice;
    char *choiceList[] = {"4 x 4", "5 x 5", "6 x 6"};
    choice = showMenu("Choose the size", choiceList, sizeof(choiceList) / sizeof(choiceList[0]));
    switch(choice)
    {
        case 0: //4*4
            grid->cols = 4;
            grid->rows = 4;
            break;

        case 1: //5*5
            grid->cols = 5;
            grid->rows = 5;
            break;

        case 2: //6*6
            grid->cols = 6;
            grid->rows = 6;
            break;
    }

    initializeGrid(grid);
    playGame(grid);
}

/**
 * initialisation de la grille de jeux
 * @author Hugo
 * @brief initialisation d'une grille
 * @param *grid --> Grid : accedes a la structure grid (cursorRow,cursorCol...)
*/
void initializeGrid(Grid *grid) {

    grid->moves = 0;

    grid->lights = (int **)malloc(grid->rows * sizeof(int *));
    for (int i = 0; i < grid->rows; ++i) {
        grid->lights[i] = (int *)malloc(grid->rows* sizeof(int));
        for (int j=0; j<grid->cols; j++)
            grid->lights[i][j]=0;
    }

    for (int i=0; i < grid->rows * grid->cols; i++)
    {
        toggleCell(grid, rand() % (grid->rows) , rand() % (grid->cols) );
    }

    grid->cursorRow = 0;
    grid->cursorCol = 0;


}

void initializeCustomGrid(Grid *grid)
{
    //Ask for the grid size

    bool validCols = false, validRows = false;

    curs_set(2);
    while (!validCols)
    {
        printw("Enter the number of columns ");
        scanw("%i", &grid->cols);
        if (grid->cols < MIN_SIZE || grid->cols > MAX_SIZE)
            printw("Invalid size!\n");
        else
            validCols = true;

    }
    while (!validRows)
    {
        printw("\nEnter the number of rows ");
        scanw("%i", &grid->rows);
        if (grid->rows < MIN_SIZE || grid->rows > MAX_SIZE)
            printw("Invalid size!\n");
        else
            validRows = true;
    }
    curs_set(0);

    grid->moves = 0;

    // Allocation dynamique des tableaux 2D
    grid->lights = (int **)malloc(grid->rows * sizeof(int *));
    for (int i = 0; i < grid->rows; ++i) {
        grid->lights[i] = (int *)malloc(grid->cols * sizeof(int));
        for (int j=0; j < grid->cols; j++)
            grid->lights[i][j]=0;

    }

    for (int i=0; i < grid->rows * grid->cols; i++)
    {
        toggleCell(grid, rand() % (grid->rows) , rand() % (grid->cols) );
    }

    // Initialise le curseur au coin supérieur gauche
    grid->cursorRow = 0;
    grid->cursorCol = 0;

}

void printGrid(Grid *grid) {
    int startX = (COLS - (grid->cols * 3)) / 2;  // (COLS - largeur_totale_de_la_grille) / 2
    int startY = (LINES - grid->rows) / 2;  // (LINES - hauteur_totale_de_la_grille) / 2

    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);  // Définir la paire de couleur 1 comme jaune sur fond noir

    for (int i = 0; i < grid->rows; ++i) {
        for (int j = 0; j < grid->cols; ++j) {
            if (grid->lights[i][j] == 1) {
                attron(COLOR_PAIR(1));
                mvprintw(startY + i, startX + j * 3, "[%c]", (grid->lights[i][j] == 1) ? 'X' : ' ');
                attroff(COLOR_PAIR(1));
            }

            else {
                mvprintw(startY + i, startX + j * 3, "[%c]", (grid->lights[i][j] == 1) ? 'X' : ' ');
            }
        }
    }
    refresh();
}

void printCursor(Grid *grid)
{
    int startX = (COLS - (grid->cols * 3)) / 2;  // (COLS - largeur_totale_de_la_grille) / 2
    int startY = (LINES - grid->rows) / 2;  // (LINES - hauteur_totale_de_la_grille) / 2
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    if (grid->lights[grid->cursorRow][grid->cursorCol] == 1)
    {
        attron(COLOR_PAIR(1));
        attron(A_BOLD);
        mvprintw(startY + grid->cursorRow, startX + grid->cursorCol * 3, "[#]");
        attroff(A_BOLD);
        attroff(COLOR_PAIR(1));
    }

    else
    {
        attron(A_BOLD);
        mvprintw(startY + grid->cursorRow, startX + grid->cursorCol * 3, "[#]");
        attroff(A_BOLD);
    }
    refresh();
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
/**
 * Permet de verifier sur le jeux est finis ou pas, GROSOMODO REGARDER SI LES LUMIERES SONT ETEINTES
 * @author Hugo
 * @brief Toutes les ligts eteintes ?
 * @param *grid --> Grid : accedes a la structure grid (cursorRow,cursorCol...)
 * @param row --> int : ligne de la grille
 * @param col --> int : colonne de la grille
*/
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
/**
 * Permet de sauvegarder la partit dans un fichier
 * @author Hugo
 * @brief sauvegarde de l'utilisateur de la partit
 * @param *grid --> Grid : accedes a la structure grid (cursorRow,cursorCol...)
 * @param *filename --> char : nom du ficher a sauvegarder (autosave.txt)
*/
void saveGame(Grid *grid, char *filename) {
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "%d %d %d\n", grid->rows, grid->cols, grid->moves);
        for (int i = 0; i < grid->rows; ++i) {
            for (int j = 0; j < grid->cols; ++j) {
                fprintf(file, "%d ", grid->lights[i][j]);
            }
            fprintf(file, "\n");
        }
        fclose(file);

        // Mettre à jour la variable saveSuccess
        grid->saveSuccess = 1;

        mvprintw(LINES - 1, 0, "Game saved successfully!");
        refresh();
        // usleep(300000); // Pause de 0.3 seconde (en microsecondes)
    } else {
        // Mettre à jour la variable saveSuccess
        grid->saveSuccess = 0;

        mvprintw(LINES - 1, 0, "Error saving game.");
        refresh();
        usleep(300000); // Pause de 0.3 seconde (en microsecondes)
    }
    clear();  // Effacer le message après la pause
    refresh();
}
/**
 * Permet de sauvegarder la grille a chaque modification
 * @author Hugo
 * @brief sauvegarde automatique de la partit
 * @param *grid --> Grid : accedes a la structure grid (cursorRow,cursorCol...)
 * @param *filname --> char :  nom du fichier dans le quelle la sauvegarde auto se fait
*/
 void autoSaveGame(Grid *grid, char *filename) {
     // Sauvegarde automatique de la partie en cours
     saveGame(grid, filename);
 }
/**
 * Permet de chager le fichier qui contient les infos de la grille
 * @author Hugo
 * @brief chargement de l'utilisateur de la partit
 * @param *grid --> Grid : accedes a la structure grid (cursorRow,cursorCol...)
 * @param *filename --> char : nom du fichuer a charger
*/
void loadGame(Grid *grid, char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fscanf(file, "%d %d %d", &(grid->rows), &(grid->cols), &(grid->moves));

    grid->lights = (int **)malloc(grid->rows * sizeof(int *));
    for (int i = 0; i < grid->rows; ++i) {
        grid->lights[i] = (int *)malloc(grid->cols * sizeof(int));
        for (int j=0; j < grid->cols; j++)
            grid->lights[i][j]=0;

    }

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
/**
 * Represente l'ensemble des options du menu, en mettant en surbrillance le choix selectioner
 * @author Hugo
 * @brief Effet visuel du menu
 * @param highlight --> int : tableau des choix du menue (Nouvell Partie, Continue...)
 * @param *choices[] --> char : tableau de chaînes de caractères représentant les différentes options du menu.
 * @param numChoices --> int
*/
void drawMenu(int highlight, char *choices[], int numChoices) {
    int x, y, i;

    x = (COLS - 20) / 2; // Centrer horizontalement
    y = (LINES - numChoices) / 2; // Centrer verticalement

    for (i = 0; i < numChoices; ++i) {
        if (highlight == i) {
            attron(A_REVERSE); //surbrillance on
            mvprintw(y, x, "  > %s", choices[i]); // Ajout de l'espace avant l'option sélectionnée
            attroff(A_REVERSE); //surbrillance off
        } else {
            mvprintw(y, x, "   %s", choices[i]); // Ajout de l'espace avant les autres options
        }
        ++y;
    }

    refresh();
}

/**
 * Utilisation de ncurses pour mise en page (gras, couleur, centrage etc...)
 * @author Hugo
 * @brief Affiche le menu
 * @param *title -> char : Titre Menue Principal
 * @param *choices[] -> char : tableau des choix du menue (Nouvell Partie, Continue...)
 * @param numChoices -> int :  représente le nombre d'options dans le menu
*/
int showMenu(char *title, char *choices[], int numChoices) {
    int highlight = 0;
    int choice;
    int c;

    do {
        clear();

        attron(A_BOLD);  // Activer le texte en gras
        mvhline((LINES - numChoices) / 2 - 3, (COLS - strlen(title)) / 2 - 2, '-', strlen(title) + 4); // Dessiner la bordure supérieure du titre
        mvprintw((LINES - numChoices) / 2 - 2, (COLS - strlen(title)) / 2 - 2, "| %s |", title);// Dessiner le titre avec la bordure latérale
        mvhline((LINES - numChoices) / 2 - 1, (COLS - strlen(title)) / 2 - 2, '-', strlen(title) + 4);// Dessiner la bordure inférieure du titre
        attroff(A_BOLD);  // desactive le texte en gras

        drawMenu(highlight, choices, numChoices);

        c = getch();
        switch (c) {
            case KEY_UP:
                if (highlight > 0) {
                    do {
                        highlight--;
                    } while (strlen(choices[highlight]) == 0);  // Ignore les options vides
                }
                break;
            case KEY_DOWN:
                if (highlight < numChoices - 1) {
                    do {
                        highlight++;
                    } while (strlen(choices[highlight]) == 0);  // Ignore les options vides
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

/**
 * Permet d'afficher Light's Out au milleu de l'ecran au dememrrage du jeux
 * @author Hugo
 * @brief Affiche Lights out au demarrage du jeux
 */
void showTitleScreen() {
    clear();
    mvprintw(LINES / 2, (COLS - 10) / 2, "Lights Out");
    refresh();
    getch();
}

/**
 * verifie si le fichier d'autosave existe (permet de savoir quand on doit afficher continuer, et sauver la partit)
 * @author Hugo
 * @brief Fichier de sauvegarder existant ?
 * @param *filename -> char : le nom du fichier
*/
bool hasSaveFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return true;
    }
    return false;
}
