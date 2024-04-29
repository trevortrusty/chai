#ifndef POSITION_DISPLAY_H
#define POSITION_DISPLAY_H

#include <ncurses.h>
#include "Screen.h"

int positionDisplay(WINDOW * win, WINDOW * editor, Screen &Screen, int &cursorY, int &cursorX, int topLine);

#endif
