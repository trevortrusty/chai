#include <iostream>
#include <ncurses.h>

#include "../src/Screen.h"

void resetTestEnv(WINDOW *editor, Screen &Screen, std::vector<std::string> &lines, std::string &line)
{
    wclear(editor);
    Screen.propsDefaultValues();
    lines.clear();
    line = "";
    lines.push_back(line);
}

int testCharInput(WINDOW * editor, Screen &Screen, std::vector<std::string> &lines, std::string &line, std::string &substringUpToEnter, std::string &substrAfterEnter, char test_value)
{
    int ascii_value = (int)test_value;
    Screen.keyCharPrint(editor, Screen, lines, line, substringUpToEnter, substrAfterEnter, ascii_value);
    char charAtCursor = mvwinch(editor, Screen.currentLine + Screen.TOP_SPACING - Screen.topLine, Screen.LEFT_SPACING);
    char charStored = lines.at(Screen.currentLine - 1)[Screen.currentChar - 2];
    return test_value == charAtCursor && test_value==charStored;
}

int testMoveAfterInput(WINDOW * editor, Screen &Screen, std::vector<std::string> &lines, std::string &line, std::string &substringUpToEnter, std::string &substrAfterEnter, std::string test_value)
{
    int ascii_value;
    for(int i = 0; i < test_value.length(); ++i)
    {
        ascii_value = test_value[i];
        Screen.keyCharPrint(editor, Screen, lines, line, substringUpToEnter, substrAfterEnter, ascii_value);
    }
    
    
    std::string lineStored = lines.at(Screen.currentLine - 1);
    printw("%s : %s", test_value.c_str(), lineStored.c_str());
    return test_value == lineStored && Screen.currentChar == test_value.length() + 1;
}

int main()
{
    Screen Screen;
    initscr();
    noecho();
    keypad(stdscr, true);
    move(0,0);

    // Create a char matrix to store all input
    std::vector<std::string> lines;
    std::string line = "";

    // Vector to store log for each test
    std::vector<std::string> logs;

    // initialize first empty line
    lines.push_back(line);



    // Get screen dimensions
    getmaxyx(stdscr, Screen.maxY, Screen.maxX);

    WINDOW *editor = newwin(Screen.maxY - 3, Screen.maxX - 3, 1, 1);
    wrefresh(editor);

    // Initialize cursor position
    Screen.curY = Screen.curX = 0;
    std::string substringUpToEnter, substrAfterEnter;

    // Enable scrolling
    scrollok(editor, TRUE);
    wsetscrreg(editor, Screen.TOP_SPACING, Screen.maxY - 1);

    // WINDOW * pos = newwin(3, 100, Screen.maxY - 1, 4);
    // positionDisplay(pos, editor, Screen, Screen.currentLine, Screen.currentChar, Screen.topLine);
    // wrefresh(pos);

    // Start line numbering
    mvwprintw(editor, Screen.TOP_SPACING,1,"%*d ", 3, Screen.currentLine);
    wrefresh(editor);

    // Start text input with a bit of a left + top margin
    wmove(editor, Screen.TOP_SPACING, Screen.LEFT_SPACING);


    // Test input of single character to the screen with an empty/new file
    if(testCharInput(editor, Screen, lines, line, substringUpToEnter, substrAfterEnter, 'b'))
    {
        //printw("testCharInput: Passed\n");
        logs.push_back("testCharInput: Passed");
    } 
    else {
        // printw("testCharInput: Failed\n");
        logs.push_back("testCharInput: Failed");
    }

    resetTestEnv(editor, Screen, lines, line);

    // Test whether cursor moves to the right while inputting characters by putting several characters to the screen
    if(testMoveAfterInput(editor, Screen, lines, line, substringUpToEnter, substrAfterEnter, "abcde"))
    {
        // printw("testMoveAfterInput: Passed\n");
        logs.push_back("testMoveAfterInput: Passed\n");
    } 
    else {
        // printw("testMoveAfterInput: Failed\n");
        logs.push_back("testMoveAfterInput: Failed\n");
    }

    // Clean up
    endwin();
    delwin(editor);

    // Display Tests
    for(std::string msg : logs)
    {
        std::cout << msg << std::endl;
    }
    // std::cout << "hello" << std::endl;
    std::cin.get();
}