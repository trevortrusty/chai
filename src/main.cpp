#include <iostream>
#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "positionDisplay.h"
#include "splash.h"
#include "Screen.h"

// Text editor line numbering starts at 1, this function takes a line number and makes it compatible with zero-based indexing
int zeroBased(int indexToChange)
{
    return indexToChange - 1;
}

int main(int argc, char *argv[])
{
    std::ifstream file;
    std::string filename = argc > 1? argv[1] : "";
    std::string line_from_file;

    Screen Screen;
    initscr();
    noecho();
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
    std::string line;

    WINDOW *editor = newwin(Screen.getMaxTermY() - 3, Screen.getMaxTermX() - 3, 1, 1);
    keypad(editor, true);
    std::string substringUpToEnter, substrAfterEnter;

    // Enable scrolling
    scrollok(editor, TRUE);
    wsetscrreg(editor, Screen.TOP_SPACING, Screen.getMaxTermY() - 1);

    if(argc == 1) // no filename provided by user
    {
        line = "";
        // initialize first empty line
        lines.push_back(line);
        mvwprintw(editor, Screen.TOP_SPACING,1,"%*d ", 3, Screen.getCurrentLinePos());
    }
    else {
        try {
            file.open(filename);
            if(file.bad())
            {
                throw 1;
            }
        }
        catch (int err) {
            file.close();
        }
    }
    if (file.is_open())
    {
        // Initialize at zero to allow pre-incrementing when printing each line out.
        Screen.setCurrentLinePos(0);
        while (getline(file,line_from_file))
        {
            lines.push_back(line_from_file);
        }
        file.close();
        for(std::string l : lines)
        {
            Screen.incCurrentLinePos();
            mvwprintw(editor, Screen.getCurrentLinePos() + Screen.TOP_SPACING - Screen.getTopLinePos(), 1, "%*d  %s", 3,  Screen.getCurrentLinePos(), l.c_str());
        }
        Screen.setNumberOfLines(lines.size());
        // Screen.autoSetEditorDimensions(editor);
        Screen.setBottomLinePos(std::min(Screen.getMaxEditorY(editor), Screen.getNumberOfLines()));
        timeout(200);
        getch();
        timeout(-1);
    }
    else {
        wmove(editor, Screen.TOP_SPACING, Screen.LEFT_SPACING);
    }
    Screen.setCurrentLinePos(1);
    Screen.setCurrentCharPos(1);
    line = lines[Screen.getCurrentLinePos() - 1]; // Set the current line string
    wmove(editor, Screen.getCurrentLinePos() + Screen.TOP_SPACING - Screen.getTopLinePos(), Screen.getCurrentCharPos() + Screen.LEFT_SPACING - 1);

    WINDOW * pos = newwin(3, 100, Screen.getMaxTermY() - 1, 4);
    positionDisplay(pos, editor, Screen);
    wrefresh(editor);

    // Input character
    char ch;
    int c;

    while(exit_condition == 0 && (c = wgetch(editor)))
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
        positionDisplay(pos, editor, Screen);
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
    return 0;
}