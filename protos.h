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

// Game's prototype functions
void initializeGrid(Grid *grid);
void initializeCustomGrid(Grid *grid);
void initializeLightsDefault (Grid *grid);
void initializeLightsCustom (Grid *grid);
void printGrid(Grid *grid);
void printCursor(Grid *grid);
void toggleCell(Grid *grid, int row, int col);
int isGameOver(Grid *grid);
void saveGame(Grid *grid, char *filename);
void loadGame(Grid *grid, char *filename);
void loadAutoSavedGame(Grid *grid, char *filename);

// Menu's prototype functions
void drawMenu(int highlight, char *choices[], int numChoices);
int showMenu(char *title, char *choices[], int numChoices);
void showTitleScreen();
bool hasSaveFile(const char *filename);
#endif //PROTOS_H
