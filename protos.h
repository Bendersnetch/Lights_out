#ifndef PROTOS_H
#define PROTOS_H

#define MAX_SIZE 11
#define MIN_SIZE 4

typedef struct {
    int rows;
    int cols;
    int lights[MAX_SIZE][MAX_SIZE];
    int cursorRow;
    int cursorCol;
} Grid;

// Prototypes de fonctions pour le jeu
void initializeGrid(Grid *grid);
void printGrid(Grid *grid);
void toggleCell(Grid *grid, int row, int col);
int isGameOver(Grid *grid);
void saveGame(Grid *grid, char *filename);
void autoSaveGame(Grid *grid, char *filename);
void loadGame(Grid *grid, char *filename);
void loadAutoSavedGame(Grid *grid, char *filename);

// Prototypes de fonctions pour le menu
void drawMenu(int highlight, char *choices[], int numChoices);
int showMenu(char *title, char *choices[], int numChoices);
#endif //PROTOS_H