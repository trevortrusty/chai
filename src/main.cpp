#include <iostream>
#include <ncurses.h>

int splash();

// int digits(int x)
// {
//     return ((std::string)x).length();
// }

// void increaseLeftSpacing

int main()
{
    // Display Splash Screen
    // splash();

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);

    // Get screen dimensions
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    
    // Initialize cursor position
    int y, x;
    y = x = 0;

    // Input character
    char ch;
    int c;

    // Set variables for spacing
    int leftSpacing = 6;
    int topSpacing = 2;

    int numberOfLines = 1;
    int currentLine = 1;
    int currentChar = 1;

    // Start line numbering
    mvprintw(topSpacing,1,"%*d ", 3, currentLine);

    // Start text input with a bit of a left + top margin
    move(topSpacing, leftSpacing);

    while(c = getch())
    {
        switch(c)
        {
            // Handle enter key press, give new line proper left spacing
            case 10:
                // Get cursor position and print line number to the next line
                getyx(stdscr, y, x);
                mvprintw(y + 1, 1, "%*d", 3,  y + 1);
                move(y + 1, leftSpacing);
                break;
            default:
                printw("%c", (char)c);
        }
    }
    

    getch();
    endwin();
    return 0;
}
































































































































