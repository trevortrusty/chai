#include <iostream>
#include <ncurses.h>
#include <vector>
#include <string>
#include "positionDisplay.h"
#include "splash.h"
#include "Screen.h"

// const int TOP_SPACING = 0;
// const int LEFT_SPACING = 6;

// Text editor line numbering starts at 1, this function takes a line number and makes it compatible with zero-based indexing
int zeroBased(int indexToChange)
{
    return indexToChange - 1;
}


int main(int argc, char **argv[])
{
    Screen Screen;
    initscr();
    // getch();
    noecho();
    // cbreak();
    keypad(stdscr, true);
    move(0,0);
    int exit_condition = 0;

    // Display Splash Screen
    if(argc == 1)
    {
        splash();
    }
    else {
        timeout(200);
        getch();
        timeout(-1);
    }
    

    // Create a char matrix to store all input
    std::vector<std::string> lines;
    std::string line = "";

    // initialize first empty line
    lines.push_back(line);



    // Get screen dimensions
    // int maxY, maxX, winMaxY, winMaxX;
    getmaxyx(stdscr, Screen.maxY, Screen.maxX);

    WINDOW *editor = newwin(Screen.maxY - 3, Screen.maxX - 3, 1, 1);
    // box(editor, 0, 0);
    wrefresh(editor);

    // Initialize cursor position
    // int y, x;
    Screen.curY = Screen.curX = 0;

    // // Set variables for spacing
    // int leftSpacing = 6;
    // int topSpacing = 0;

    // int numberOfLines = 1;
    // int currentLine = 1;
    // int currentChar = 1;
    // int topLine = 1; // Keep track of the top visible line when scrolling
    // int printToLine;
    std::string substringUpToEnter, substrAfterEnter;

    // Enable scrolling
    scrollok(editor, TRUE);
    wsetscrreg(editor, Screen.TOP_SPACING, Screen.maxY - 1);
    // idlok(editor, TRUE);

    WINDOW * pos = newwin(3, 100, Screen.maxY - 1, 4);
    positionDisplay(pos, editor, Screen, Screen.currentLine, Screen.currentChar, Screen.topLine);
    // wrefresh(pos);

    // Start line numbering
    mvwprintw(editor, Screen.TOP_SPACING,1,"%*d ", 3, Screen.currentLine);
    // wrefresh(editor);
    // Start text input with a bit of a left + top margin
    wmove(editor, Screen.TOP_SPACING, Screen.LEFT_SPACING);
    wrefresh(pos);
    wrefresh(editor);
    // Input character
    char ch;
    int c;

    while(exit_condition == 0 && (c = getch()))
    {
        switch(c)
        {
            // Handle up arrow
            case KEY_UP:
                Screen.keyUp(editor, Screen, lines, line);
                break;
                
            case KEY_DOWN:
                Screen.keyDown(editor, Screen, lines, line);
                break;

            case KEY_LEFT:
                Screen.keyLeft(editor, Screen, lines, line);
                break;
            
            case KEY_RIGHT:
                Screen.keyRight(editor, Screen, lines, line);
                break;
            case KEY_BACKSPACE:
                Screen.keyBackspace(editor, Screen, lines, line);
                break;
            case '\t':
                Screen.keyTab(editor, Screen, lines, line);
                break;
            // Handle enter key press, give new line proper left spacing
            case 10:
                Screen.keyEnter(editor, Screen, lines, line, substringUpToEnter, substrAfterEnter);
                break;
            
            case KEY_F(8):
                exit_condition = 1;
                break;

            default:
                Screen.keyCharPrint(editor, Screen, lines, line, substringUpToEnter, substrAfterEnter, c);
        }
        // wmove(pos, getcury(pos), getcurx(pos)-3);
        // wrefresh(pos);
        positionDisplay(pos, editor, Screen, Screen.curY, Screen.curX, Screen.topLine);
        wrefresh(pos);
        wrefresh(editor);
    }
    // Clean up
    delwin(editor);
    delwin(pos);
    endwin();

    for(int i = 0; i < lines.size(); i++)
    {
        std::cout << lines[i] << std::endl;
    }
    // getch();
    return 0;
}