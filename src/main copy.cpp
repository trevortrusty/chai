#include <iostream>
#include <ncurses.h>
#include <vector>
#include "positionDisplay.h"

int splash();

int main()
{
    int exit_condition = 0;

    // Display Splash Screen
    splash();

    std::vector<std::string> lines;
    std::string line = "";

    // initialize first empty line
    lines.push_back(line);

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);

    // Get screen dimensions
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    // Initialize cursor position
    int y, x;
    y = x = 0;

    // Input character
    char ch;
    int c;

    // Set variables for spacing
    int leftSpacing = 6;
    int topSpacing = 0;

    int numberOfLines = 1;
    int currentLine = 1;
    int currentChar = 1;
    
    // Enable scrolling
    scrollok(stdscr, TRUE);
    setscrreg(topSpacing, maxY);
    idlok(stdscr, TRUE);

    WINDOW * pos = newwin(3, 20, maxY - 1, 4);
    positionDisplay(pos, currentLine, currentChar);
    wrefresh(pos);
    // Start line numbering
    mvprintw(topSpacing,1,"%*d ", 3, currentLine);

    // Start text input with a bit of a left + top margin
    move(topSpacing, leftSpacing);

    while(exit_condition == 0 && (c = getch()))
    {
        switch(c)
        {
            // Handle up arrow
            case KEY_UP:
                lines[currentLine - 1] = line;
                if(currentLine > 1)
                {
                    currentLine--;
                    // Check if the previous line up is shorter than the current line. 
                    // If it is shorter, cursor moves to end of the previous line in order to stay in bounds
                    if(currentChar > lines[currentLine - 1].length())
                    {
                        currentChar = lines[currentLine - 1].length() + 1;
                        move(currentLine + topSpacing - 1, lines[currentLine - 1].length() + leftSpacing);
                    }
                    else {
                        move(currentLine + topSpacing - 1, currentChar + leftSpacing - 1);
                    }
                    line = lines[currentLine - 1];

                    // Get cursor position, if cursor at or above top of scrolling region then scroll
                    getyx(stdscr, y, x);
                    if(y < topSpacing)
                    {
                        wscrl(stdscr, -1); // Scroll the window up by one line                            
                    }
                }
                else
                {
                    // Can't move up any farther, move cursor to beginning of current line
                    currentChar = 1;
                    move(topSpacing, leftSpacing);
                }
                break;


            
            case KEY_DOWN:
                lines[currentLine - 1] = line;
                if(currentLine != numberOfLines)
                {
                    currentLine++;
                    // Check if the next line down is shorter than the current line. 
                    // If it is shorter, cursor moves to end of the next line in order to stay in bounds
                    if(currentChar > lines[currentLine - 1].length())
                    {
                        currentChar = lines[currentLine - 1].length() + 1;
                        move(currentLine + topSpacing - 1, lines[currentLine - 1].length() + leftSpacing);
                    }
                    else {
                        move(currentLine + topSpacing - 1, currentChar + leftSpacing - 1);
                    }
                    line = lines[currentLine - 1];

                    // Get curor pos + window size, if cursor at bottom then scroll
                    getyx(stdscr, y, x);
                    getmaxyx(stdscr, maxY, maxX);
                    if(y >= maxY)
                    {
                        wscrl(stdscr, 1); // Scroll the window down by one line                            
                    }
                }
                else
                {
                    // Can't move down any farther, move cursor to beginning of current line
                    currentChar = 1;
                    move(currentLine + topSpacing - 1, leftSpacing);
                }
                break;

            case KEY_LEFT:
                if(currentChar != 1)
                {
                    currentChar--;
                    move(currentLine + topSpacing - 1, currentChar + leftSpacing - 1);
                }
                else {
                    move(currentLine + topSpacing - 1, currentChar + leftSpacing - 1);
                }
                break;
            
            case KEY_RIGHT:
                if(currentChar <= line.length())
                {
                    currentChar++;
                    move(currentLine + topSpacing - 1, currentChar + leftSpacing - 1);
                }
                break;
            // Handle enter key press, give new line proper left spacing
            case 10:
                // Get cursor position and print line number to the next line
                lines[currentLine - 1] = line;
                line = "";

                if(lines.size() == numberOfLines)
                {
                    lines.push_back(line);

                    // Get curor pos + window size, if cursor at bottom then scroll
                    getyx(stdscr, y, x);
                    getmaxyx(stdscr, maxY, maxX);
                    // if(y >= maxY - 1)
                    if(y >= maxY - 2)
                    {
                        wscrl(stdscr, 1); // Scroll the window down by one line 
                        move(y - 2, leftSpacing);                           
                    }
                    else {
                        move(y + 1, leftSpacing);
                    }
                }
                else {
                    lines.insert(lines.begin() + (currentLine + 1), line);
                    move(y + 1, leftSpacing);
                }
                
                

                currentLine++;
                currentChar = 1;
                numberOfLines++;
                // getyx(stdscr, y, x);
                mvprintw(numberOfLines + topSpacing - 1, 1, "%*d", 3,  numberOfLines);
                refresh();
                // move(y + 1, leftSpacing);
                break;
            
            case KEY_F(8):
                exit_condition = 1;
                break;

            default:
                // line = line + (char)c;
                line.insert(line.begin() + currentChar - 1, (char)c);
                currentChar++;
                // printw("%c", (char)c);
                insch((char)c);
                getyx(stdscr, y, x);
                move(y, x + 1);
        }
        // wmove(pos, getcury(pos), getcurx(pos)-3);
        wrefresh(pos);
        positionDisplay(pos, currentLine, currentChar);
        wrefresh(pos);
    }
    
    endwin();
    return 0;
}
































































































































