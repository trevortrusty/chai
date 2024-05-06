#include <ncurses.h>
#include <string>
#include "Screen.h"

int positionDisplay(WINDOW * win, WINDOW * editor, Screen &Screen)
{
    wclrtoeol(win); // Clear the entire line
    box(win, 0, 0);
    std::string positionText = std::to_string(Screen.getCurrentLinePos()) + "," + std::to_string(Screen.getCurrentCharPos()) + "          " + std::to_string(Screen.getCurY(editor)) + ", " + std::to_string(Screen.getCurX(editor)) + "          " + std::to_string(Screen.getMaxEditorY(editor)) + ", " + std::to_string(Screen.getMaxEditorX(editor)) + "        Top Line, Bottom Line: " + std::to_string(Screen.getTopLinePos()) + ", " + std::to_string(Screen.getBottomLinePos()) + "/" + std::to_string(Screen.getNumberOfLines()) ;
    wprintw(win, positionText.c_str());
    wrefresh(win);
    wmove(win, getcury(win), getcurx(win)-positionText.length());
    return 0;
}