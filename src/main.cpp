#include <iostream>
#include <ncurses.h>
#include <vector>
#include <string>
#include "positionDisplay.h"
#include "splash.h"
#include "Screen.h"

const int TOP_SPACING = 0;
const int LEFT_SPACING = 6;

// struct Screen
// {
//     int maxY, maxX;         // Max cursor positions for entire terminal (stdscr)
//     int winMaxY, winMaxX;   // Max cursor positions for just the editor window

//     int curY, curX;
//     int numberOfLines = 1;  // Number of lines that have been created/exist by the user
//     int currentLine = 1;    // Line the cursor is on within the text editor
//     int currentChar = 1;    // Char/space the cursor is on within the line
//     int topLine = 1;        // Keeps track of the top visible line when scrolling
//     int printToLine;        // Keeps track of where to stop reprinting lines, when a reprint is necessary
// };

// Text editor line numbering starts at 1, this function takes a line number and makes it compatible with zero-based indexing
int zeroBased(int indexToChange)
{
    return indexToChange - 1;
}

void keyUp(WINDOW * editor, Screen &Screen, std::vector<std::string> &lines, std::string &line)
{
    lines[Screen.currentLine - 1] = line;
    if(Screen.currentLine > Screen.topLine)
    {
        if(Screen.currentLine > 1)
        {
            Screen.currentLine--;
            // Check if the previous line up is shorter than the current line. 
            // If it is shorter, cursor moves to end of the previous line in order to stay in bounds
            if(Screen.currentChar > lines[Screen.currentLine - 1].length())
            {
                Screen.currentChar = lines[Screen.currentLine - 1].length() + 1;
                wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, lines[Screen.currentLine - 1].length() + LEFT_SPACING);
            }
            else {
                wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, Screen.currentChar + LEFT_SPACING - 1);
            }
            line = lines[Screen.currentLine - 1];
        }
        else
        {
            // Can't move up any farther, move cursor to beginning of current line
            Screen.currentChar = 1;
            wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, LEFT_SPACING);
        }
    }
    else {
        if(Screen.currentLine > 1)
        {
            Screen.topLine--;
            wscrl(editor, -1); // Scroll the window up by one line
            Screen.currentLine--;

            // After scrolling up, reprint line number and the text belonging to that line
            mvwprintw(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, 1, "%*d  %s", 3,  Screen.currentLine, lines[Screen.currentLine - 1].c_str());
            //mvwprintw(editor, currentLine + topSpacing - topLine, leftSpacing, lines[currentLine].c_str());

            // Check if the previous line up is shorter than the current line. 
            // If it is shorter, cursor moves to end of the previous line in order to stay in bounds
            if(Screen.currentChar > lines[Screen.currentLine - 1].length())
            {
                Screen.currentChar = lines[Screen.currentLine - 1].length() + 1;
                wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, lines[Screen.currentLine - 1].length() + LEFT_SPACING);
            }
            else {
                wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, Screen.currentChar + LEFT_SPACING - 1);
            }
            line = lines[Screen.currentLine - 1];
        }
        else
        {
            // Can't move up any farther, move cursor to beginning of current line
            Screen.currentChar = 1;
            wmove(editor, TOP_SPACING, LEFT_SPACING);
        }
    }
}

void keyDown(WINDOW * editor, Screen &Screen, std::vector<std::string> &lines, std::string &line)
{
lines[Screen.currentLine - 1] = line;
    getmaxyx(editor, Screen.winMaxY, Screen.winMaxX);
    if(Screen.currentLine < Screen.winMaxY)
    {
        if(Screen.currentLine != Screen.numberOfLines)
        {
            Screen.currentLine++;
            // Check if the next line down is shorter than the current line. 
            // If it is shorter, cursor moves to end of the next line in order to stay in bounds
            if(Screen.currentChar > lines[Screen.currentLine - 1].length())
            {
                Screen.currentChar = lines[Screen.currentLine - 1].length() + 1;
                wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, lines[Screen.currentLine - 1].length() + LEFT_SPACING);
            }
            else {
                wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, Screen.currentChar + LEFT_SPACING - 1);
            }
            line = lines[Screen.currentLine - 1];
        }
        else
        {
            // Can't move down any farther, move cursor to beginning of current line
            Screen.currentChar = 1;
            wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, LEFT_SPACING);
        }
    }
    else {
        if(Screen.currentLine != Screen.numberOfLines)
        {
            Screen.topLine++;
            wscrl(editor, 1); // Scroll the window up by one line
            Screen.currentLine++;

            // After scrolling up, reprint line number and the text belonging to that line
            mvwprintw(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, 1, "%*d  %s", 3,  Screen.currentLine, lines[Screen.currentLine - 1].c_str());

            // Check if the next line down is shorter than the current line. 
            // If it is shorter, cursor moves to end of the next line in order to stay in bounds
            if(Screen.currentChar > lines[Screen.currentLine - 1].length())
            {
                Screen.currentChar = lines[Screen.currentLine - 1].length() + 1;
                wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, lines[Screen.currentLine - 1].length() + LEFT_SPACING);
            }
            else {
                wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, Screen.currentChar + LEFT_SPACING - 1);
            }
            line = lines[Screen.currentLine - 1];
        }
        else
        {
            // Can't move down any farther, move cursor to beginning of current line
            Screen.currentChar = 1;
            wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, LEFT_SPACING);
        }                   
    }
}

void keyLeft(WINDOW * editor, Screen &Screen, std::vector<std::string> &lines, std::string &line)
{
    if(Screen.currentChar != 1)
    {
        Screen.currentChar--;
        wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, Screen.currentChar + LEFT_SPACING - 1);
    }
    else {
        wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, Screen.currentChar + LEFT_SPACING - 1);
    }
}

void keyRight(WINDOW * editor, Screen &Screen, std::vector<std::string> &lines, std::string &line)
{
    if(Screen.currentChar <= line.length())
    {
        Screen.currentChar++;
        wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, Screen.currentChar + LEFT_SPACING - 1);
    }
}

void keyEnter(WINDOW * editor, Screen &Screen, std::vector<std::string> &lines, std::string &line, std::string &substringUpToEnter, std::string &substrAfterEnter)
{
// Get cursor position and print line number to the next line
                // lines[currentLine - 1] = line;
    substringUpToEnter = line.substr(0,Screen.currentChar - 1);
    substrAfterEnter = line.substr(Screen.currentChar - 1);
    lines[Screen.currentLine - 1] = substringUpToEnter;
    // line = "";
    line = substrAfterEnter;
    getyx(editor, Screen.curY, Screen.curX);
    int editorMaxY, editorMaxX;
    getmaxyx(editor, editorMaxY, editorMaxX);

    // Checks if space is available on the visible screen to start new line
    if(Screen.currentLine < editorMaxY)
    {
        if(Screen.currentLine == Screen.numberOfLines)
        {
            lines.push_back(line);
        }
        else {
            // lines.insert(lines.begin() + (currentLine + 1), line);
            lines.insert(lines.begin() + (Screen.currentLine), line);
        }
        
        Screen.currentLine++;
        Screen.currentChar = 1;
        Screen.numberOfLines++;
        getyx(editor, Screen.curY, Screen.curX);
        mvwprintw(editor, Screen.numberOfLines + TOP_SPACING - 1, 1, "%*d", 3,  Screen.numberOfLines);
        wrefresh(editor);
        wmove(editor, Screen.curY + 1, LEFT_SPACING);
    }
    // If no space left in window for a new line, scroll window down.
    else {
        
        // wrefresh(editor);
        // wclrtoeol(editor);
        if(Screen.currentLine == Screen.numberOfLines)
        {
            lines.push_back(line);
        }
        else {
            // lines.insert(lines.begin() + (currentLine + 1), line);
            lines.insert(lines.begin() + (Screen.currentLine), line);
        }

        Screen.currentLine++;
        Screen.currentChar = 1;
        Screen.numberOfLines++;
        // getyx(editor, y, x);
        
        // wrefresh(editor);
        Screen.topLine++; // update which line is seen at the top of the window
        wscrl(editor, 1); // Scroll the window down by one line
        // mvwprintw(editor, editorMaxY - 1, 1, "%*d", 3,  numberOfLines);
        getyx(editor, Screen.curY, Screen.curX);
        wrefresh(editor);
        wmove(editor, Screen.curY, LEFT_SPACING);
    }
    wmove(editor, Screen.curY, LEFT_SPACING);
    wclrtoeol(editor);
    wclrtobot(editor);
    Screen.printToLine = Screen.numberOfLines < Screen.winMaxY ? Screen.numberOfLines : Screen.winMaxY;
    for(int i = Screen.curY - 1; i < Screen.printToLine; i++)
    {
        mvwprintw(editor, i, 1, "%*d  %s", 3,  i + 1, lines[i].c_str());
    }
    wrefresh(editor);
    // currentChar = lines[currentLine - 1].length() + 1;
    wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, Screen.currentChar + LEFT_SPACING - 1);
}

void keyCharPrint(WINDOW * editor, Screen &Screen, std::vector<std::string> &lines, std::string &line, std::string &substringUpToEnter, std::string &substrAfterEnter, int &c)
{
    // line = line + (char)c;
    getmaxyx(editor, Screen.winMaxY, Screen.winMaxX);
    line.insert(line.begin() + Screen.currentChar - 1, (char)c);
    Screen.currentChar++;
    // printw("%c", (char)c);
    winsch(editor, (char)c);
    wrefresh(editor);
    getyx(editor, Screen.curY, Screen.curX);
    wmove(editor, Screen.curY, Screen.curX + 1);
    lines[Screen.currentLine - 1] = line;
}

void keyBackspace(WINDOW * editor, Screen &Screen, std::vector<std::string> &lines, std::string &line)
{
    if(Screen.currentChar > 1)
    {
        // Screen.currentChar--;
        // wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, Screen.currentChar + LEFT_SPACING - 1);
        // wclrtobot(editor);
        // line.pop_back();
        // lines[Screen.currentLine - 1].pop_back();
        Screen.currentChar--;
        line.erase(Screen.currentChar - 1, 1);
        lines[Screen.currentLine - 1].erase(Screen.currentChar - 1, 1);
        wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, LEFT_SPACING);
        wclrtoeol(editor);

        mvwprintw(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, 1, "%*d  %s", 3,  Screen.currentLine, lines[Screen.currentLine - 1].c_str());
        wrefresh(editor);
        
        wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, Screen.currentChar + LEFT_SPACING - 1);
    }
    else {
        beep();
    }
}

void keyTab(WINDOW * editor, Screen &Screen, std::vector<std::string> &lines, std::string &line)
{
    getmaxyx(editor, Screen.winMaxY, Screen.winMaxX);
    line.insert(line.begin() + Screen.currentChar - 1, 4, ' '); // 4 Spaces
    Screen.currentChar += 4;
    // printw("%c", (char)c);
    for(int i = 0; i < 4; ++i)
    {
        winsch(editor, ' ');
    }
    wrefresh(editor);
    getyx(editor, Screen.curY, Screen.curX);
    wmove(editor, Screen.curY, Screen.curX + 4);
    lines[Screen.currentLine - 1] = line;
}

int main(int argc, char **argv[])
{
    Screen Screen;
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
    

    // Create a char matrix to store all input
    std::vector<std::string> lines;
    std::string line = "";

    // initialize first empty line
    lines.push_back(line);



    // Get screen dimensions
    // int maxY, maxX, winMaxY, winMaxX;
    getmaxyx(stdscr, Screen.maxY, Screen.maxX);

    WINDOW *editor = newwin(Screen.maxY - 3, Screen.maxX - 3, 1, 1);
    // box(editor, 0, 0);
    wrefresh(editor);

    // Initialize cursor position
    // int y, x;
    Screen.curY = Screen.curX = 0;

    // // Set variables for spacing
    // int leftSpacing = 6;
    // int topSpacing = 0;

    // int numberOfLines = 1;
    // int currentLine = 1;
    // int currentChar = 1;
    // int topLine = 1; // Keep track of the top visible line when scrolling
    // int printToLine;
    std::string substringUpToEnter, substrAfterEnter;

    // Enable scrolling
    scrollok(editor, TRUE);
    wsetscrreg(editor, TOP_SPACING, Screen.maxY - 1);
    // idlok(editor, TRUE);

    WINDOW * pos = newwin(3, 100, Screen.maxY - 1, 4);
    positionDisplay(pos, editor, Screen, Screen.currentLine, Screen.currentChar, Screen.topLine);
    wrefresh(pos);

    // Start line numbering
    mvwprintw(editor, TOP_SPACING,1,"%*d ", 3, Screen.currentLine);
    wrefresh(editor);
    // Start text input with a bit of a left + top margin
    wmove(editor, TOP_SPACING, LEFT_SPACING);

    // Input character
    char ch;
    int c;

    while(exit_condition == 0 && (c = getch()))
    {
        switch(c)
        {
            // Handle up arrow
            case KEY_UP:
                keyUp(editor, Screen, lines, line);
                break;
                
            case KEY_DOWN:
                keyDown(editor, Screen, lines, line);
                break;

            case KEY_LEFT:
                keyLeft(editor, Screen, lines, line);
                break;
            
            case KEY_RIGHT:
                keyRight(editor, Screen, lines, line);
                break;
            case KEY_BACKSPACE:
                keyBackspace(editor, Screen, lines, line);
                break;
            case '\t':
                keyTab(editor, Screen, lines, line);
                break;
            // Handle enter key press, give new line proper left spacing
            case 10:
                keyEnter(editor, Screen, lines, line, substringUpToEnter, substrAfterEnter);
                break;
            
            case KEY_F(8):
                exit_condition = 1;
                break;

            default:
                keyCharPrint(editor, Screen, lines, line, substringUpToEnter, substrAfterEnter, c);
        }
        // wmove(pos, getcury(pos), getcurx(pos)-3);
        // wrefresh(pos);
        positionDisplay(pos, editor, Screen, Screen.curY, Screen.curX, Screen.topLine);
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