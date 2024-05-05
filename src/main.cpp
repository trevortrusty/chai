#include <iostream>
#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
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

std::istream& safeGetline(std::istream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}

int main(int argc, char *argv[])
{
    std::ifstream file;
    std::string filename = argc > 1? argv[1] : "";
    std::string line_from_file;

    Screen Screen;
    initscr();
    // getch();
    noecho();
    // cbreak();
    // keypad(stdscr, true);
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

    // Get screen dimensions
    // int maxY, maxX, winMaxY, winMaxX;
    getmaxyx(stdscr, Screen.maxY, Screen.maxX);

    WINDOW *editor = newwin(Screen.maxY - 3, Screen.maxX - 3, 1, 1);
    // box(editor, 0, 0);
    // wrefresh(editor);
    keypad(editor, true);
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

    if(argc == 1) // no filename provided by user
    {
        line = "";
        // initialize first empty line
        lines.push_back(line);
        mvwprintw(editor, Screen.TOP_SPACING,1,"%*d ", 3, Screen.currentLine);
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
        Screen.currentLine = 0;
        // Screen.numberOfLines = 0;
        while (getline(file,line_from_file))
        {
            // Screen.numberOfLines++;
            // Screen.currentLine++;
            lines.push_back(line_from_file);
            // mvwprintw(editor, Screen.TOP_SPACING,Screen.currentLine,"%*d ", 3, line_from_file);
            // mvwprintw(editor, Screen.currentLine + Screen.TOP_SPACING - Screen.topLine, 1, "%*d  %s", 3,  Screen.currentLine, line_from_file.c_str());
            // mvwprintw(editor, Screen.currentLine + Screen.TOP_SPACING - Screen.topLine, 1, "%*d  %s", 3,  Screen.currentLine, lines[Screen.currentLine - 1].c_str());
        }
        file.close();
        for(std::string l : lines)
        {
            Screen.currentLine++;
            mvwprintw(editor, Screen.currentLine + Screen.TOP_SPACING - Screen.topLine, 1, "%*d  %s", 3,  Screen.currentLine, l.c_str());
        }
        Screen.numberOfLines = lines.size();
        // Screen.bottomLine = Screen.maxY - 1 - Screen.TOP_SPACING;
        // Screen.bottomLine = Screen.numberOfLines - Screen.topLine  + 1;
        // Screen.bottomLine = Screen.numberOfLines - Screen.topLine  + 1;
        getmaxyx(editor, Screen.winMaxY, Screen.winMaxX);
        Screen.bottomLine = std::min(Screen.winMaxY, Screen.numberOfLines);

        // line = lines[0]; // Set the current line string
        //Screen.currentChar = lines[Screen.currentLine - 1].length() + 1;
        timeout(200);
        getch();
        timeout(-1);
        // wrefresh(editor);
        // wmove(editor, Screen.currentLine + Screen.TOP_SPACING - Screen.topLine, lines[Screen.currentLine - 1].length() + Screen.LEFT_SPACING);
    }
    else {
        // wrefresh(editor);
        wmove(editor, Screen.TOP_SPACING, Screen.LEFT_SPACING);
    }
    Screen.currentLine = 1;
    Screen.currentChar = 1;
    line = lines[Screen.currentLine - 1]; // Set the current line string
    wmove(editor, Screen.currentLine + Screen.TOP_SPACING - Screen.topLine, Screen.currentChar + Screen.LEFT_SPACING - 1);
    // wrefresh(pos);

    // Start line numbering
    // mvwprintw(editor, Screen.TOP_SPACING,1,"%*d ", 3, Screen.currentLine);
    // wrefresh(editor);
    // Start text input with a bit of a left + top margin
    // wmove(editor, Screen.TOP_SPACING, Screen.LEFT_SPACING);
    WINDOW * pos = newwin(3, 100, Screen.maxY - 1, 4);
    positionDisplay(pos, editor, Screen, Screen.currentLine, Screen.currentChar, Screen.topLine, Screen.bottomLine);
    wrefresh(editor);
    // Input character
    char ch;
    int c;
    // Screen.keyLeft(editor, Screen, lines, line);
    // Screen.keyRight(editor, Screen, lines, line);
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
        // wmove(pos, getcury(pos), getcurx(pos)-3);
        // wrefresh(pos);
        positionDisplay(pos, editor, Screen, Screen.curY, Screen.curX, Screen.topLine, Screen.bottomLine);
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