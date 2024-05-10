#include <ncurses.h>
#include <string>
#include "TextEditor.h"

int positionDisplay(WINDOW * win, WINDOW * editor, TextEditor &TextEditor)
{
    wclrtoeol(win); // Clear the entire line
    box(win, 0, 0);
    std::string positionText = std::to_string(TextEditor.getCurrentLinePos()) + "," + std::to_string(TextEditor.getCurrentCharPos()) + "          " + std::to_string(TextEditor.getCurY(editor)) + ", " + std::to_string(TextEditor.getCurX(editor)) + "          " + std::to_string(TextEditor.getMaxEditorY(editor)) + ", " + std::to_string(TextEditor.getMaxEditorX(editor)) + "        Top Line, Bottom Line: " + std::to_string(TextEditor.getTopLinePos()) + ", " + std::to_string(TextEditor.getBottomLinePos()) + "/" + std::to_string(TextEditor.getNumberOfLines()) ;
    wprintw(win, positionText.c_str());
    wrefresh(win);
    wmove(win, getcury(win), getcurx(win)-positionText.length());
    return 0;
}