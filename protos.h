#ifndef PROTOS_H
#define PROTOS_H

#define MAX_SIZE 11
#define MIN_SIZE 4

typedef struct {
    int rows;
    int cols;
    int cursorRow;
    int cursorCol;
    int **lights;  // Utilisation d'un double pointeur pour allouer dynamiquement des tableaux 2D
    int saveSuccess;
} Grid;

// Prototypes de fonctions pour le jeu

void playGame(Grid *grid);
void initializeGrid(Grid *grid);
void printGrid(Grid *grid);
void toggleCell(Grid *grid, int row, int col);
int isGameOver(Grid *grid);
void saveGame(Grid *grid, char *filename);
void loadGame(Grid *grid, char *filename);
void loadAutoSavedGame(Grid *grid, char *filename);

// Prototypes de fonctions pour le menu

void drawMenu(int highlight, char *choices[], int numChoices);
int showMenu(char *title, char *choices[], int numChoices);
void showTitleScreen();
bool hasSaveFile(const char *filename);
#endif //PROTOS_H