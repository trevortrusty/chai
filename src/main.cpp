#include <iostream>
#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "positionDisplay.h"
#include "splash.h"
#include "TextEditor.h"

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

    TextEditor TE;
    initscr();
    noecho();
    move(0,0);
    int exit_condition = 0;

    // Display Splash/Title only for new files
    if(argc == 1)
    {
        splash();
    }
    else {
        // Delay launching text editor window by 200ms to prevent all hell from breaking loose.
        timeout(200);
        getch();
        timeout(-1);
    }
    

    WINDOW *editor = newwin(TE.getMaxTermY() - 3, TE.getMaxTermX() - 3, 1, 1);
    keypad(editor, true);
    std::string substringUpToEnter, substrAfterEnter;

    // Enable scrolling
    scrollok(editor, TRUE);
    wsetscrreg(editor, TE.TOP_SPACING, TE.getMaxTermY() - 1);

    // Open file for editing, if one was provided from cmdline
    if(argc == 1) // no filename provided by user
    {
        // Setting initial line input buffer as empty string, and pushing it to the data structure
        TE.setEmptyLineBuffer();
        TE.pushLine();
        mvwprintw(editor, TE.TOP_SPACING,1,"%*d ", 3, TE.getCurrentLinePos());      // Print initial line number ("1. ")
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
        TE.setCurrentLinePos(0);

        // Push all lines from input file
        while (getline(file,line_from_file))
        {
            TE.pushLine(line_from_file);
        }
        file.close();

        // Print all lines, each starting with the line number
        for(std::string l : TE.getAllLines())
        {
            TE.incCurrentLinePos();
            mvwprintw(editor, TE.getCurrentLinePos() + TE.TOP_SPACING - TE.getTopLinePos(), 1, "%*d  %s", 3,  TE.getCurrentLinePos(), l.c_str());
        }

        TE.setNumberOfLines(TE.getAllLines().size());
        TE.setBottomLinePos(std::min(TE.getMaxEditorY(editor), TE.getNumberOfLines()));
        timeout(200);
        getch();
        timeout(-1);
    }
    else {
        wmove(editor, TE.TOP_SPACING, TE.LEFT_SPACING);
    }

    // Tells the application the cursor is to be placed at file start (first char of the first line)
    TE.setCurrentLinePos(1);
    TE.setCurrentCharPos(1);

    // Prepare the first line for user input
    TE.setLineBuffer(TE.lineAt(TE.getCurrentLinePos() - 1));
    wmove(editor, TE.getCurrentLinePos() + TE.TOP_SPACING - TE.getTopLinePos(), TE.getCurrentCharPos() + TE.LEFT_SPACING - 1);


    // Set up display of the cursor position at the bottom of the screen
    WINDOW * pos = newwin(3, 100, TE.getMaxTermY() - 1, 4);
    positionDisplay(pos, editor, TE);
    wrefresh(editor);

    // Input character
    char ch;
    int c;


    // User input loop
    while(exit_condition == 0 && (c = wgetch(editor)))
    {
        switch(c)
        {
            // Handle up arrow
            case KEY_UP:
                TE.keyUp(editor);
                break;
                
            case KEY_DOWN:
                TE.keyDown(editor);
                break;

            case KEY_LEFT:
                TE.keyLeft(editor);
                break;
            
            case KEY_RIGHT:
                TE.keyRight(editor);
                break;
            case KEY_BACKSPACE:
                TE.keyBackspace(editor);
                break;
            case '\t':
                TE.keyTab(editor);
                break;
            // Handle enter key press, give new line proper left spacing
            case 10:
                TE.keyEnter(editor, substringUpToEnter, substrAfterEnter);
                break;
            
            case KEY_F(8):
                exit_condition = 1;
                break;

            default:
                TE.keyCharPrint(editor, substringUpToEnter, substrAfterEnter, c);
        }
        positionDisplay(pos, editor, TE);
        wrefresh(pos);
        wrefresh(editor);
    }
    // Clean up
    delwin(editor);
    delwin(pos);
    endwin();

    for(int i = 0; i < TE.getAllLines().size(); i++)
    {
        std::cout << TE.lineAt(i) << std::endl;
    }
    return 0;
}
