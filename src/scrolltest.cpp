#include <iostream>
#include <ncurses.h>

int main() {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();

    // Enable scrolling
    scrollok(stdscr, TRUE);

    // Print some lines to fill the screen
    for (int i = 0; i < LINES; i++) {
        printw("Line %d\n", i + 1);
    }

    // Refresh the screen to display the lines
    refresh();

    // Wait for user input
    getch();

    // End ncurses
    endwin();

    return 0;
}
