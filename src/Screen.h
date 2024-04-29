#ifndef SCREEN_H
#define SCREEN_H

struct Screen
{
    int maxY, maxX;         // Max cursor positions for entire terminal (stdscr)
    int winMaxY, winMaxX;   // Max cursor positions for just the editor window

    int curY, curX;
    int numberOfLines = 1;  // Number of lines that have been created/exist by the user
    int currentLine = 1;    // Line the cursor is on within the text editor
    int currentChar = 1;    // Char/space the cursor is on within the line
    int topLine = 1;        // Keeps track of the top visible line when scrolling
    int printToLine;        // Keeps track of where to stop reprinting lines, when a reprint is necessary
};

#endif
