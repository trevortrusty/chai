#include <ncurses.h>
#include <string>

int positionDisplay(WINDOW * win, int cursorY, int cursorX)
{
    // WINDOW * pos = newwin(3, 3, maxY - 20, 4);
    wclrtoeol(win); // Clear the entire line
    std::string positionText = std::to_string(cursorY) + "," + std::to_string(cursorX);
    // wprintw(win, "%d,%d", cursorY, cursorX);
    wprintw(win, positionText.c_str());
    wrefresh(win);
    wmove(win, getcury(win), getcurx(win)-positionText.length());
    return 0;
}