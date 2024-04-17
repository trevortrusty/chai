#include <string>
#include <ncurses.h>


int splash()
{
    // Set dimensions for box
    int boxHeight = 10;
    int boxWidth = 30;

    std::string title = "Chai";
    std::string subtitle = "Text Editor";

    // Initialize ncurses screen
    initscr();
    noecho();
    
    // Get the terminal screen dimensions
    int height, width;
    getmaxyx(stdscr, height, width);

    // Create boxed window, centered in terminal screen
    WINDOW * w = newwin(boxHeight, boxWidth, (height / 2) - (boxHeight / 2), (width / 2) - (boxWidth / 2));
    refresh();

    box(w, (int)'=',(int)'+');
    mvwprintw(w, 3, boxWidth/2 - title.length()/2, title.c_str());
    mvwprintw(w, 5, boxWidth/2 - subtitle.length()/2, subtitle.c_str());
    wrefresh(w);

    move(0,0);
    getch();
    
    // No more screen.
    endwin();
    delwin(w);
    return 0;
}