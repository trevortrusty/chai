#include <iostream>
#include <ncurses.h>
#include <vector>
#include "positionDisplay.h"

int splash();

int main()
{
    // Display Splash Screen
    // splash();
    std::vector<std::string> lines;
    std::string line = "";

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
    int topSpacing = 1;

    int numberOfLines = 1;
    int currentLine = 1;
    int currentChar = 1;
    
    WINDOW * pos = newwin(3, 20, maxY - 1, 4);
    positionDisplay(pos, currentLine, currentChar);
    wrefresh(pos);
    // Start line numbering
    mvprintw(topSpacing,1,"%*d ", 3, currentLine);

    // Start text input with a bit of a left + top margin
    move(topSpacing, leftSpacing);

    while(c = getch())
    {
        switch(c)
        {
            // Handle up arrow
            case KEY_UP:
                if(currentLine > 1)
                {
                    currentLine--;
                    move(currentLine + topSpacing - 1, currentChar + leftSpacing - 1);
                }
                else
                {
                    currentChar = 1;
                    move(topSpacing, leftSpacing);
                }
                break;
            
            case KEY_DOWN:
                if(currentLine != numberOfLines)
                {
                    currentLine++;
                    move(currentLine + topSpacing - 1, currentChar + leftSpacing - 1);
                }
                else
                {
                    currentChar = 1;
                    move(currentLine + topSpacing - 1, leftSpacing);
                }
                break;

            case KEY_LEFT:
                if(currentChar != 1)
                {
                    currentChar--;
                    move(currentLine + topSpacing - 1, currentChar + leftSpacing - 1);
                }
                else {
                    move(currentLine + topSpacing - 1, currentChar + leftSpacing - 1);
                }
                break;
            
            case KEY_RIGHT:
                if(true)
                {
                    currentChar++;
                    move(currentLine + topSpacing - 1, currentChar + leftSpacing - 1);
                }
                break;
            // Handle enter key press, give new line proper left spacing
            case 10:
                // Get cursor position and print line number to the next line
                currentLine++;
                currentChar = 1;
                numberOfLines++;
                getyx(stdscr, y, x);
                mvprintw(numberOfLines + topSpacing - 1, 1, "%*d", 3,  numberOfLines);
                move(y + 1, leftSpacing);
                break;
            default:
                line = line + (char)c;
                currentChar++;
                printw("%c", (char)c);
        }
        // wmove(pos, getcury(pos), getcurx(pos)-3);
        wrefresh(pos);
        positionDisplay(pos, currentLine, currentChar);
        wrefresh(pos);
    }
    

    getch();
    endwin();
    return 0;
}
































































































































