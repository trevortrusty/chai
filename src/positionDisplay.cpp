#include <ncurses.h>
#include <string>
#include "Screen.h"

int positionDisplay(WINDOW * win, WINDOW * editor, Screen &Screen, int &cursorY, int &cursorX, int topLine)
{
    int scrY, scrX, maxY, maxX;
    getyx(editor, scrY, scrX);
    getmaxyx(editor, maxY, maxX);
    // WINDOW * pos = newwin(3, 3, maxY - 20, 4);
    wclrtoeol(win); // Clear the entire line
    box(win, 0, 0);
    // std::string positionText = std::to_string(cursorY) + "," + std::to_string(cursorX) + "          " + std::to_string(scrY) + ", " + std::to_string(scrX) + "          " + std::to_string(maxY) + ", " + std::to_string(maxX) + "        Top Line: " + std::to_string(topLine);
    std::string positionText = std::to_string(Screen.currentLine) + "," + std::to_string(Screen.currentChar) + "          " + std::to_string(scrY) + ", " + std::to_string(scrX) + "          " + std::to_string(maxY) + ", " + std::to_string(maxX) + "        Top Line: " + std::to_string(topLine);
    // wprintw(win, "%d,%d", cursorY, cursorX);
    wprintw(win, positionText.c_str());
    wrefresh(win);
    wmove(win, getcury(win), getcurx(win)-positionText.length());
    return 0;
}