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

// Game's prototype functions
void initializeGrid(Grid *grid);
void initializeCustomGrid(Grid *grid);
void printGrid(Grid *grid);
void printCursor(Grid *grid);
void toggleCell(Grid *grid, int row, int col);
int isGameOver(Grid *grid);
void saveGame(Grid *grid, char *filename);
void autoSaveGame(Grid *grid, char *filename);
void loadGame(Grid *grid, char *filename);
void loadAutoSavedGame(Grid *grid, char *filename);

// Menu's prototype functions
void drawMenu(int highlight, char *choices[], int numChoices);
int showMenu(char *title, char *choices[], int numChoices);
bool hasSaveFile(const char *filename);
#endif //PROTOS_H
