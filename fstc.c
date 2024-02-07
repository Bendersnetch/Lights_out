#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <ncurses.h>
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
 * Base gameplay function
 * @author Hugo, Federico
 * @brief Gameplay
 * @param *grid --> Grid : uses the grid structure (cursorRow,cursorCol...)
 */
void playGame(Grid *grid)
{
    int gameOver = 0;
    int input;

    do
    {
        clear();
        // Show the name and moves count
        mvprintw((LINES - grid->rows) / 2 - 5, (COLS - 10) / 2, "Lights Out");
        mvprintw((LINES - grid->rows) / 2 - 4, (COLS - 27) / 2, "You have already done %d moves", grid->moves);

        mvprintw((LINES - grid->rows) / 2 - 1, 3, "Instructions: ");
        mvprintw((LINES - grid->rows) / 2 + 1, 3, "Use the arrows to navigate");
        mvprintw((LINES - grid->rows) / 2 + 3, 3, "Press enter to change a light's state");
        mvprintw((LINES - grid->rows) / 2 + 5, 3, "Press esc to return to the menu");

        // show rules on the right
        mvprintw((LINES - grid->rows) / 2 - 1, COLS - 35, "Rules:");
        mvprintw((LINES - grid->rows) / 2 + 1, COLS - 35, "Turn off all lights to win");

        // show the game grid
        printGrid(grid);

        printCursor(grid);

        input = getch();

        switch (input)
        {
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
            grid->moves++;
            toggleCell(grid, grid->cursorRow, grid->cursorCol);
            gameOver = isGameOver(grid);
            break;
        case 27: // Escape key
            saveGame(grid, "autosave.txt");
            return; // Returns to the menu
        default:
            break;
        }

    } while (!gameOver);

    clear();
    mvprintw(LINES / 2, (COLS - 62) / 2, "Congratulations! You've turned off all the lights in %d moves.", grid->moves);
    saveScore(grid);
    refresh();
    getch(); // Wait to the user to press a key
}
/**
 * Secondary menu used to choose the size of the base game
 * @author Federico
 * @brief size Menu
 * @param *grid --> Grid : uses the grid structure (rows,cols)
 */
void chooseSize(Grid *grid)
{
    int choice;
    char *choiceList[] = {"4 x 4", "5 x 5", "6 x 6"};
    choice = showMenu("Choose the size", choiceList, sizeof(choiceList) / sizeof(choiceList[0]));
    switch (choice)
    {
    case 0: // 4*4
        grid->cols = 4;
        grid->rows = 4;
        break;

    case 1: // 5*5
        grid->cols = 5;
        grid->rows = 5;
        break;

    case 2: // 6*6
        grid->cols = 6;
        grid->rows = 6;
        break;
    }

    initializeGrid(grid);
    playGame(grid);
}

/**
 * Secondary menu used to choose the size of the custom grid game
 * @author Hugo
 * @brief size Menu
 * @param *grid --> Grid : uses the grid structure (rows,cols)
 */
void chooseCustomSize(Grid *grid)
{
    clear();
    mvprintw(LINES / 2 - 4 , COLS / 2 - 15, "CHOOSE CUSTOM SIZE");
    bool validCols = false, validRows = false;

    curs_set(2);

    int maxCols = COLS / 2;
    int maxRows = LINES / 2;

    int colPrompt = maxCols - 20; // Calculer la position horizontale du texte
    int rowPrompt = maxRows - 1;  // Calculer la position verticale du texte

    while (!validCols)
    {
        mvprintw(rowPrompt, colPrompt, "Enter the number of columns ");
        scanw("%i", &grid->cols);
        if (grid->cols < MIN_SIZE)
            mvprintw(rowPrompt + 1, colPrompt, "Invalid size! Too small, try a value higher than %d", MIN_SIZE);
        else if (grid->cols > MAX_SIZE)
            mvprintw(rowPrompt + 1, colPrompt, "Invalid size! Too big, try a value smaller than %d", MAX_SIZE);
        else
            validCols = true;
    }

    while (!validRows)
    {
        mvprintw(rowPrompt + 2, colPrompt, "Enter the number of rows ");
        scanw("%i", &grid->rows);
        if (grid->rows < MIN_SIZE)
            mvprintw(rowPrompt + 3, colPrompt, "Invalid size! Too small, try a value higher than %d", MIN_SIZE);
        else if (grid->rows > MAX_SIZE)
            mvprintw(rowPrompt + 3, colPrompt, "Invalid size! Too big, try a value smaller than %d", MAX_SIZE);
        else
            validRows = true;
    }

    curs_set(0);

    grid->moves = 0;
    initializeCustomGrid(grid);
    playGame(grid);
}

/**
 * Initialize the grid for a new game
 * @author Hugo, Federico
 * @brief initilaize base grid
 * @param *grid --> Grid : uses the grid structure (rows,cols...)
 */
void initializeGrid(Grid *grid)
{

    grid->moves = 0;

    // dynamic allocation for the grid
    grid->lights = (int **)malloc(grid->rows * sizeof(int *));
    for (int i = 0; i < grid->rows; ++i)
    {
        grid->lights[i] = (int *)malloc(grid->rows * sizeof(int));
        for (int j = 0; j < grid->cols; j++)
            grid->lights[i][j] = 0;
    }

    // randomize the grid lights
    for (int i = 0; i < grid->rows * grid->cols; i++)
    {
        toggleCell(grid, rand() % (grid->rows), rand() % (grid->cols));
    }

    // Initialize the cursor in the top left corner
    grid->cursorRow = 0;
    grid->cursorCol = 0;
}

/**
 * Initialize the grid for a new custom game
 * @author Federico
 * @brief initilaize custom grid
 * @param *grid --> Grid : uses the grid structure (rows,cols...)
 */
void initializeCustomGrid(Grid *grid)
{
    // dynamic allocation for the grid
    grid->lights = (int **)malloc(grid->rows * sizeof(int *));
    for (int i = 0; i < grid->rows; ++i)
    {
        grid->lights[i] = (int *)malloc(grid->cols * sizeof(int));
        for (int j = 0; j < grid->cols; j++)
            grid->lights[i][j] = 0;
    }

    // randomize the grid lights
    for (int i = 0; i < grid->rows * grid->cols; i++)
    {
        toggleCell(grid, rand() % (grid->rows), rand() % (grid->cols));
    }

    // Initialize the cursor in the top left corner
    grid->cursorRow = 0;
    grid->cursorCol = 0;
}

/**
 * Show the grid
 * @author Hugo
 * @brief show grid
 * @param *grid --> Grid : uses the grid structure (rows,cols...)
 */
void printGrid(Grid *grid)
{
    int startX = (COLS - (grid->cols * 3)) / 2;
    int startY = (LINES - grid->rows) / 2;

    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Defines the color combination (yellow and black)

    for (int i = 0; i < grid->rows; ++i)
    {
        for (int j = 0; j < grid->cols; ++j)
        {
            if (grid->lights[i][j] == 1)
            {
                attron(COLOR_PAIR(1));
                mvprintw(startY + i, startX + j * 3, "[%c]", (grid->lights[i][j] == 1) ? 'X' : ' ');
                attroff(COLOR_PAIR(1));
            }

            else
            {
                mvprintw(startY + i, startX + j * 3, "[%c]", (grid->lights[i][j] == 1) ? 'X' : ' ');
            }
        }
    }
    refresh();
}

/**
 * Show the cursor in the grid
 * @author Federico
 * @brief show cursor
 * @param *grid --> Grid : uses the grid structure (rows,cols...)
 */
void printCursor(Grid *grid)
{
    int startX = (COLS - (grid->cols * 3)) / 2;
    int startY = (LINES - grid->rows) / 2;
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

/**
 * change the state of a cell and his neighbours
 * @author Hugo
 * @brief change light
 * @param *grid --> Grid : uses the grid structure (rows,cols...)
 */
void toggleCell(Grid *grid, int row, int col)
{
    // Change the light state and his neighbours state
    grid->lights[row][col] = 1 - grid->lights[row][col];

    if (row > 0)
    {
        grid->lights[row - 1][col] = 1 - grid->lights[row - 1][col];
    }

    if (row < grid->rows - 1)
    {
        grid->lights[row + 1][col] = 1 - grid->lights[row + 1][col];
    }

    if (col > 0)
    {
        grid->lights[row][col - 1] = 1 - grid->lights[row][col - 1];
    }

    if (col < grid->cols - 1)
    {
        grid->lights[row][col + 1] = 1 - grid->lights[row][col + 1];
    }
}

/**
 * Checks if all the lights are off
 * @author Hugo
 * @brief LightsOff?
 * @param *grid --> Grid : uses the grid structure (rows,cols...)
 */
int isGameOver(Grid *grid)
{
    for (int i = 0; i < grid->rows; ++i)
    {
        for (int j = 0; j < grid->cols; ++j)
        {
            if (grid->lights[i][j] == 1)
            {
                return 0; // Game isn't over
            }
        }
    }

    return 1; // Game is over
}
/**
 * Save the game in a file
 * @author Hugo
 * @brief save
 * @param *grid --> Grid : uses the grid structure (rows,cols...)
 * @param *filename --> char : file's name (autosave.txt)
 */
void saveGame(Grid *grid, char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file != NULL)
    {
        fprintf(file, "%d %d %d\n", grid->rows, grid->cols, grid->moves);
        for (int i = 0; i < grid->rows; ++i)
        {
            for (int j = 0; j < grid->cols; ++j)
            {
                fprintf(file, "%d ", grid->lights[i][j]);
            }
            fprintf(file, "\n");
        }
        fclose(file);

        // Change the success variable
        grid->saveSuccess = 1;

        mvprintw(LINES - 1, 0, "Game saved successfully!");
        refresh();
    }
    else
    {
        // Change the success variable
        grid->saveSuccess = 0;

        mvprintw(LINES - 1, 0, "Error saving game.");
        refresh();
    }
    clear(); // erase the message after 0.3 seconds
    refresh();
}

void saveScore(Grid *grid) {
    FILE *file = fopen("scoreboard.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %d %d %d\n", grid->playerName, grid->moves, grid->rows, grid->cols);
        fclose(file);
    }
}



/**
 * Load the game from a file
 * @author Hugo
 * @brief load
 * @param *grid --> Grid : uses the grid structure (rows,cols...)
 * @param *filename --> char : file's name (autosave.txt)
 */
void loadGame(Grid *grid, char *filename)
{
    int rowPrompt = LINES / 2 - 1;
    int colPrompt = COLS / 2 - 15;
    FILE *file = fopen(filename, "r");
    if (file != NULL)
    {
        fscanf(file, "%d %d %d", &(grid->rows), &(grid->cols), &(grid->moves));

        grid->lights = (int **)malloc(grid->rows * sizeof(int *));
        for (int i = 0; i < grid->rows; ++i)
        {
            grid->lights[i] = (int *)malloc(grid->cols * sizeof(int));
            for (int j = 0; j < grid->cols; j++)
                grid->lights[i][j] = 0;
        }

        for (int i = 0; i < grid->rows; ++i)
        {
            for (int j = 0; j < grid->cols; ++j)
            {
                fscanf(file, "%d", &(grid->lights[i][j]));
            }
        }
        fclose(file);
        clear();
        mvprintw(rowPrompt, colPrompt, "Game loaded successfully!");
    }
    else
    {
        mvprintw(rowPrompt, colPrompt, "Error loading game.");
    }
    refresh();
}

/**
 * Show the menus options and higlight the one selected
 * @author Hugo
 * @brief Menu
 * @param highlight --> int : array with the menu choices
 * @param *choices[] --> char : array with the menu choices (New game, Continue...)
 * @param numChoices --> int
 */
void drawMenu(int highlight, char *choices[], int numChoices)
{
    int x, y, i;

    x = (COLS - 20) / 2;          // Centrer horizontally
    y = (LINES - numChoices) / 2; // Centrer vertically

    for (i = 0; i < numChoices; ++i)
    {
        if (highlight == i)
        {
            attron(A_REVERSE);                   // highlight on
            mvprintw(y, x, " > %s", choices[i]); // Add the spsace before the selected option
            attroff(A_REVERSE);                  // highlight off
        }
        else
        {
            mvprintw(y, x, "   %s", choices[i]); // add the space before the other choices
        }
        ++y;
    }

    refresh();
}

/**
 * Use curses library to show elements
 * @author Hugo
 * @brief show menu
 * @param *title -> char : Menu title
 * @param *choices[] -> char : array with the menu choices (New game, Continue...)
 * @param numChoices -> int :  choices nulber of the menu
 */
int showMenu(char *title, char *choices[], int numChoices)
{
    int highlight = 0;
    int choice;
    int c;

    do
    {
        clear();

        attron(A_BOLD);                                                                                // Activer le texte en gras
        mvhline((LINES - numChoices) / 2 - 3, (COLS - strlen(title)) / 2 - 2, '-', strlen(title) + 4); // Dessiner la bordure supérieure du titre
        mvprintw((LINES - numChoices) / 2 - 2, (COLS - strlen(title)) / 2 - 2, "| %s |", title);       // Dessiner le titre avec la bordure latérale
        mvhline((LINES - numChoices) / 2 - 1, (COLS - strlen(title)) / 2 - 2, '-', strlen(title) + 4); // Dessiner la bordure inférieure du titre
        attroff(A_BOLD);                                                                               // desactive le texte en gras

        drawMenu(highlight, choices, numChoices);

        c = getch();
        switch (c)
        {
        case KEY_UP:
            if (highlight > 0)
            {
                do
                {
                    highlight--;
                } while (strlen(choices[highlight]) == 0); // Ignore les options vides
            }
            break;
        case KEY_DOWN:
            if (highlight < numChoices - 1)
            {
                do
                {
                    highlight++;
                } while (strlen(choices[highlight]) == 0); // Ignore the empty options
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

void showScoreboard() {
    clear();
    mvprintw(1, (COLS - 15) / 2, "=== Scoreboard ===");
    //mvprintw(3, (COLS - 20) / 2, "Player   Moves   Grid Size");
    FILE *file = fopen("scoreboard.txt", "r");
    if (file != NULL) {
        int row = 5;
        char line[100];
        int taille = 3;

        char *title = "Player   Moves   Grid Size";
        attron(A_BOLD);                                                                                // Activer le texte en gras
        mvhline(taille, (COLS - strlen(title)) / 2 - 2, '-', strlen(title) + 4); // Dessiner la bordure supérieure du titre
        mvprintw(taille + 1, (COLS - strlen(title)) / 2 - 2, "| %s |", title);       // Dessiner le titre avec la bordure latérale
        mvhline(taille + 2, (COLS - strlen(title)) / 2 - 2, '-', strlen(title) + 4); // Dessiner la bordure inférieure du titre
        attroff(A_BOLD); 

        while (fgets(line, sizeof(line), file) != NULL) {
            // Split the line into name, moves, and size
            char playerName[50];
            int moves, cols, rows;  
            sscanf(line, "%s %d %d %d", playerName, &moves, &cols, &rows);
            mvprintw(row + 1, (COLS - 25) / 2, "%s", playerName);
            mvprintw(row + 1, (COLS - 5) / 2, "%d", moves, cols, rows);
            mvprintw(row + 1, (COLS + 10) / 2, "%dx%d", cols, rows);


            row++;
        }

        fclose(file);  
    } else {
        mvprintw(3, (COLS - 30) / 2, "No scores available.");
    }

    mvprintw(LINES - 1, 0, "Press any key to return to the menu.");
    refresh();
    getch(); // Attend que l'utilisateur appuie sur une touche

}


/**
 * Show 'lightsout' n the middle of a screen
 * @author Hugo
 * @brief show lightsout
 */
void showTitleScreen()
{
    clear();
    mvprintw(LINES / 2, (COLS - 10) / 2, "Lights Out");
    mvprintw(LINES / 2 + 2, (COLS - 22) / 2, "Press any key to start...");
    refresh();
    getch();
}

void getPlayerName(Grid *grid) {
    clear();
    mvprintw(LINES / 2, (COLS - 30) / 2, "Enter your name and press Enter:");
    refresh();
    echo();
    curs_set(1);
    getnstr(grid->playerName, sizeof(grid->playerName));
    curs_set(0);
    noecho();
}


/**
 * checks if there is a save file (tells if we have to show continue in the menu)
 * @author Hugo
 * @brief is there a save File?
 * @param *filename -> char : file's name
 */
bool hasSaveFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file != NULL)
    {
        fclose(file);
        return true;
    }
    return false;
}
