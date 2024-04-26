#include <iostream>
#include <ncurses.h>
#include <vector>
#include "positionDisplay.h"
#include <string>

const int TOP_SPACING = 0;
const int LEFT_SPACING = 6;

int splash();

void keyUp(WINDOW * editor, std::vector<std::string> &lines, std::string &line, int &currentLine, int &topLine, int &currentChar)
{
    lines[currentLine - 1] = line;
    if(currentLine > topLine)
    {
        if(currentLine > 1)
        {
            currentLine--;
            // Check if the previous line up is shorter than the current line. 
            // If it is shorter, cursor moves to end of the previous line in order to stay in bounds
            if(currentChar > lines[currentLine - 1].length())
            {
                currentChar = lines[currentLine - 1].length() + 1;
                wmove(editor, currentLine + TOP_SPACING - topLine, lines[currentLine - 1].length() + LEFT_SPACING);
            }
            else {
                wmove(editor, currentLine + TOP_SPACING - topLine, currentChar + LEFT_SPACING - 1);
            }
            line = lines[currentLine - 1];
        }
        else
        {
            // Can't move up any farther, move cursor to beginning of current line
            currentChar = 1;
            wmove(editor, currentLine + TOP_SPACING - topLine, LEFT_SPACING);
        }
    }
    else {
        if(currentLine > 1)
        {
            topLine--;
            wscrl(editor, -1); // Scroll the window up by one line
            currentLine--;

            // After scrolling up, reprint line number and the text belonging to that line
            mvwprintw(editor, currentLine + TOP_SPACING - topLine, 1, "%*d  %s", 3,  currentLine, lines[currentLine - 1].c_str());
            //mvwprintw(editor, currentLine + topSpacing - topLine, leftSpacing, lines[currentLine].c_str());

            // Check if the previous line up is shorter than the current line. 
            // If it is shorter, cursor moves to end of the previous line in order to stay in bounds
            if(currentChar > lines[currentLine - 1].length())
            {
                currentChar = lines[currentLine - 1].length() + 1;
                wmove(editor, currentLine + TOP_SPACING - topLine, lines[currentLine - 1].length() + LEFT_SPACING);
            }
            else {
                wmove(editor, currentLine + TOP_SPACING - topLine, currentChar + LEFT_SPACING - 1);
            }
            line = lines[currentLine - 1];
        }
        else
        {
            // Can't move up any farther, move cursor to beginning of current line
            currentChar = 1;
            wmove(editor, TOP_SPACING, LEFT_SPACING);
        }
    }
}

void keyDown(WINDOW * editor, std::vector<std::string> &lines, std::string &line, int &currentLine, int &topLine, int &currentChar, int &winMaxY, int &winMaxX, int &numberOfLines)
{
lines[currentLine - 1] = line;
    getmaxyx(editor, winMaxY, winMaxX);
    if(currentLine < winMaxY)
    {
        if(currentLine != numberOfLines)
        {
            currentLine++;
            // Check if the next line down is shorter than the current line. 
            // If it is shorter, cursor moves to end of the next line in order to stay in bounds
            if(currentChar > lines[currentLine - 1].length())
            {
                currentChar = lines[currentLine - 1].length() + 1;
                wmove(editor, currentLine + TOP_SPACING - topLine, lines[currentLine - 1].length() + LEFT_SPACING);
            }
            else {
                wmove(editor, currentLine + TOP_SPACING - topLine, currentChar + LEFT_SPACING - 1);
            }
            line = lines[currentLine - 1];
        }
        else
        {
            // Can't move down any farther, move cursor to beginning of current line
            currentChar = 1;
            wmove(editor, currentLine + TOP_SPACING - topLine, LEFT_SPACING);
        }
    }
    else {
        if(currentLine != numberOfLines)
        {
            topLine++;
            wscrl(editor, 1); // Scroll the window up by one line
            currentLine++;

            // After scrolling up, reprint line number and the text belonging to that line
            mvwprintw(editor, currentLine + TOP_SPACING - topLine, 1, "%*d  %s", 3,  currentLine, lines[currentLine - 1].c_str());

            // Check if the next line down is shorter than the current line. 
            // If it is shorter, cursor moves to end of the next line in order to stay in bounds
            if(currentChar > lines[currentLine - 1].length())
            {
                currentChar = lines[currentLine - 1].length() + 1;
                wmove(editor, currentLine + TOP_SPACING - topLine, lines[currentLine - 1].length() + LEFT_SPACING);
            }
            else {
                wmove(editor, currentLine + TOP_SPACING - topLine, currentChar + LEFT_SPACING - 1);
            }
            line = lines[currentLine - 1];
        }
        else
        {
            // Can't move down any farther, move cursor to beginning of current line
            currentChar = 1;
            wmove(editor, currentLine + TOP_SPACING - topLine, LEFT_SPACING);
        }                   
    }
}

void keyLeft(WINDOW * editor, std::vector<std::string> &lines, std::string &line, int &currentLine, int &topLine, int &currentChar)
{
    if(currentChar != 1)
    {
        currentChar--;
        wmove(editor, currentLine + TOP_SPACING - topLine, currentChar + LEFT_SPACING - 1);
    }
    else {
        wmove(editor, currentLine + TOP_SPACING - topLine, currentChar + LEFT_SPACING - 1);
    }
}

void keyRight(WINDOW * editor, std::vector<std::string> &lines, std::string &line, int &currentLine, int &topLine, int &currentChar)
{
    if(currentChar <= line.length())
    {
        currentChar++;
        wmove(editor, currentLine + TOP_SPACING - topLine, currentChar + LEFT_SPACING - 1);
    }
}

void keyEnter(WINDOW * editor, std::vector<std::string> &lines, std::string &line, int &currentLine, int &topLine, int &currentChar, int &winMaxY, int &winMaxX, int &numberOfLines, std::string &substringUpToEnter, std::string &substrAfterEnter, int &curY, int &curX, int &printToLine)
{
// Get cursor position and print line number to the next line
                // lines[currentLine - 1] = line;
    substringUpToEnter = line.substr(0,currentChar - 1);
    substrAfterEnter = line.substr(currentChar - 1);
    lines[currentLine - 1] = substringUpToEnter;
    // line = "";
    line = substrAfterEnter;
    getyx(editor, curY, curX);
    int editorMaxY, editorMaxX;
    getmaxyx(editor, editorMaxY, editorMaxX);

    // Checks if space is available on the visible screen to start new line
    if(currentLine < editorMaxY)
    {
        if(currentLine == numberOfLines)
        {
            lines.push_back(line);
        }
        else {
            // lines.insert(lines.begin() + (currentLine + 1), line);
            lines.insert(lines.begin() + (currentLine), line);
        }
        
        currentLine++;
        currentChar = 1;
        numberOfLines++;
        getyx(editor, curY, curX);
        mvwprintw(editor, numberOfLines + TOP_SPACING - 1, 1, "%*d", 3,  numberOfLines);
        wrefresh(editor);
        wmove(editor, curY + 1, LEFT_SPACING);
    }
    // If no space left in window for a new line, scroll window down.
    else {
        
        // wrefresh(editor);
        // wclrtoeol(editor);
        if(currentLine == numberOfLines)
        {
            lines.push_back(line);
        }
        else {
            // lines.insert(lines.begin() + (currentLine + 1), line);
            lines.insert(lines.begin() + (currentLine), line);
        }

        currentLine++;
        currentChar = 1;
        numberOfLines++;
        // getyx(editor, y, x);
        
        // wrefresh(editor);
        topLine++; // update which line is seen at the top of the window
        wscrl(editor, 1); // Scroll the window down by one line
        // mvwprintw(editor, editorMaxY - 1, 1, "%*d", 3,  numberOfLines);
        getyx(editor, curY, curX);
        wrefresh(editor);
        wmove(editor, curY, LEFT_SPACING);
    }
    wmove(editor, curY, LEFT_SPACING);
    wclrtoeol(editor);
    wclrtobot(editor);
    printToLine = numberOfLines < winMaxY ? numberOfLines : winMaxY;
    for(int i = curY - 1; i < printToLine; i++)
    {
        mvwprintw(editor, i, 1, "%*d  %s", 3,  i + 1, lines[i].c_str());
    }
    wrefresh(editor);
    // currentChar = lines[currentLine - 1].length() + 1;
    wmove(editor, currentLine + TOP_SPACING - topLine, currentChar + LEFT_SPACING - 1);
}

void keyCharPrint(WINDOW * editor, std::vector<std::string> &lines, std::string &line, int &currentLine, int &topLine, int &currentChar, int &winMaxY, int &winMaxX, int &numberOfLines, std::string &substringUpToEnter, std::string &substrAfterEnter, int &curY, int &curX, int &printToLine, int &c)
{
    // line = line + (char)c;
    getmaxyx(editor, winMaxY, winMaxX);
    line.insert(line.begin() + currentChar - 1, (char)c);
    currentChar++;
    // printw("%c", (char)c);
    winsch(editor, (char)c);
    wrefresh(editor);
    getyx(editor, curY, curX);
    wmove(editor, curY, curX + 1);
    lines[currentLine - 1] = line;
}

int main(int argc, char **argv[])
{
    initscr();
    // getch();
    noecho();
    // cbreak();
    keypad(stdscr, true);
    move(0,0);
    int exit_condition = 0;

    // Display Splash Screen
    if(argc == 1)
    {
        splash();
    }
    else {
        timeout(200);
        getch();
        timeout(-1);
    }
    

    std::vector<std::string> lines;
    std::string line = "";

    // initialize first empty line
    lines.push_back(line);



    // Get screen dimensions
    int maxY, maxX, winMaxY, winMaxX;
    getmaxyx(stdscr, maxY, maxX);

    WINDOW *editor = newwin(maxY - 3, maxX - 3, 1, 1);
    // box(editor, 0, 0);
    wrefresh(editor);

    // Initialize cursor position
    int y, x;
    y = x = 0;

    // Set variables for spacing
    int leftSpacing = 6;
    int topSpacing = 0;

    int numberOfLines = 1;
    int currentLine = 1;
    int currentChar = 1;
    int topLine = 1; // Keep track of the top visible line when scrolling
    int printToLine;
    std::string substringUpToEnter, substrAfterEnter;

    // Enable scrolling
    scrollok(editor, TRUE);
    wsetscrreg(editor, topSpacing, maxY - 1);
    // idlok(editor, TRUE);

    WINDOW * pos = newwin(3, 100, maxY - 1, 4);
    positionDisplay(pos, editor, currentLine, currentChar, topLine);
    wrefresh(pos);

    // Start line numbering
    mvwprintw(editor, topSpacing,1,"%*d ", 3, currentLine);
    wrefresh(editor);
    // Start text input with a bit of a left + top margin
    wmove(editor, topSpacing, leftSpacing);

    // Input character
    char ch;
    int c;

    while(exit_condition == 0 && (c = getch()))
    {
        switch(c)
        {
            // Handle up arrow
            case KEY_UP:
                keyUp(editor, lines, line, currentLine, topLine, currentChar);
                break;
                
            case KEY_DOWN:
                keyDown(editor, lines, line, currentLine, topLine, currentChar, winMaxY, winMaxX, numberOfLines);
                break;

            case KEY_LEFT:
                keyLeft(editor, lines, line, currentLine, topLine, currentChar);
                break;
            
            case KEY_RIGHT:
                keyRight(editor, lines, line, currentLine, topLine, currentChar);
                break;
            // Handle enter key press, give new line proper left spacing
            case 10:
                keyEnter(editor, lines, line, currentLine, topLine, currentChar, winMaxY, winMaxX, numberOfLines, substringUpToEnter, substrAfterEnter, y, x, printToLine);
                break;
            
            case KEY_F(8):
                exit_condition = 1;
                break;

            default:
                keyCharPrint(editor, lines, line, currentLine, topLine, currentChar, winMaxY, winMaxX, numberOfLines, substringUpToEnter, substrAfterEnter, y, x, printToLine, c);
        }
        // wmove(pos, getcury(pos), getcurx(pos)-3);
        // wrefresh(pos);
        positionDisplay(pos, editor, currentLine, currentChar, topLine);
        wrefresh(pos);
        wrefresh(editor);
    }
    // Clean up
    delwin(editor);
    delwin(pos);
    endwin();

    for(int i = 0; i < lines.size(); i++)
    {
        std::cout << lines[i] << std::endl;
    }
    getch();
    return 0;
}