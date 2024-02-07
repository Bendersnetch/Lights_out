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
#include <locale.h>
#include "protos.h"

#define rowPrompt LINES / 2 - 1
#define colPrompt COLS / 2 - 15

// Centered title screen
void showTitleScreen();

// Fonction for the gameplay
void playGame(Grid *grid);

int main()
{
 // Initialize the grid with malloc
    Grid *grid = (Grid *)malloc(sizeof(Grid));
    if (grid == NULL)
    {
        perror("Error while allocating memory");
        endwin();
        return 1;
    }

    srand(time(NULL));

    initscr(); // Initialize curses library
    curs_set(0);
    start_color(); // Activate colors
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    setlocale(LC_ALL, "");
    keypad(stdscr, TRUE);

    int choice;
    char filename[50];
    char playerName[50];

    showTitleScreen();
    getPlayerName(grid);

    

    char *menuChoices[] = {
        "New game",
        "New custom game",
        "Load game",
        "Continue game",
        "Save Game",
        "Show Scoreboard",
        "Leave"};

    int numMenuChoices = sizeof(menuChoices) / sizeof(menuChoices[0]);   

    do
    {
        clear();

        // Only show "continue" if there's a save file
        if (hasSaveFile("autosave.txt"))
        {
            menuChoices[3] = "Continue game";
            menuChoices[4] = "Save Game";
        }
        else
        {
            menuChoices[3] = ""; // Empty option if no file
            menuChoices[4] = "";
        }

        choice = showMenu("Main menu", menuChoices, numMenuChoices);
        switch (choice)
        {
        case 0: // Start New Game
            chooseSize(grid);
            break;

        case 1: // Create a custom sized game
            chooseCustomSize(grid);
            break;

        case 2: // Load Game
            clear();
            curs_set(2);
            mvprintw(LINES / 2 - 4 , COLS / 2 - 37, "If the selected file is incorrect, you will be redirected to the current game. ");
            mvprintw(rowPrompt, colPrompt, "Enter the file name: ");
            refresh();
            getstr(filename);
            curs_set(0);
            loadGame(grid, filename);
            playGame(grid);
            break;

        case 3:                             // Continue Game
            loadGame(grid, "autosave.txt"); // Load the autosave
            if (strlen(menuChoices[2]) > 0)
            {
                playGame(grid);
                break;
            }

        case 4: // Save Game
            loadGame(grid, "autosave.txt"); // Load the autosave
            if (hasSaveFile("autosave.txt"))
            {
                clear();
                curs_set(2);
                mvprintw(rowPrompt, colPrompt,("Enter the file name to save:  "));
                refresh();
                getstr(filename);
                curs_set(0);
                saveGame(grid, filename);
                break;
            }
        case 5: // Show Scoreboard
            showScoreboard();
            break;

        case 6: // Quit
            break;

        default:
            printw("Invalid choice, please retry.\n");
            refresh();
        }

    } while (choice != numMenuChoices - 1);

    // Free the used memory
    for (int i = 0; i < grid->rows; ++i)
    {
        free(grid->lights[i]);
    }
    free(grid->lights);

    free(grid);

    endwin(); // Stop curses library utilization

    return 0;
}
