#ifndef PROTOS_H
#define PROTOS_H

#define MAX_SIZE 11
#define MIN_SIZE 4

typedef struct {
    int rows;
    int cols;
    int moves;
    int cursorRow;
    int cursorCol;
    int **lights;  // Used to allocate the memory dynamicly
    int saveSuccess;
} Grid;

// Game's prototype functions
void chooseSize(Grid *grid);
void chooseCustomSize(Grid *grid);
void initializeGrid(Grid *grid);
void initializeCustomGrid(Grid *grid);
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
