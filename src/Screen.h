#ifndef SCREEN_H
#define SCREEN_H

#include <ncurses.h>
#include <vector>
#include <string>

class Screen
{
    public:
        const int TOP_SPACING = 0;
        const int LEFT_SPACING = 6;
        
        int maxY, maxX;         // Max cursor positions for entire terminal (stdscr)
        int winMaxY, winMaxX;   // Max cursor positions for just the editor window

        int curY, curX;
        int numberOfLines = 1;  // Number of lines that have been created/exist by the user
        int currentLine = 1;    // Line the cursor is on within the text editor
        int currentChar = 1;    // Char/space the cursor is on within the line
        int topLine = 1;        // Keeps track of the top visible line when scrolling
        int bottomLine = 1;     // Keeps track of the bottom visible line when scrolling
        int printToLine;        // Keeps track of where to stop reprinting lines, when a reprint is necessary

        Screen()
        {
            propsDefaultValues();
        }

        void propsDefaultValues()
        {
            numberOfLines = 1;
            currentChar = 1;
            topLine = 1;
            bottomLine = 1;
            currentLine = 1; 
        }

        void moveToStart(WINDOW * editor)
        {

        }

        void reprintVisible(WINDOW * editor, std::vector<std::string> lines)
        {
            // wmove();
            wclear(editor);
            int line_number;
            // for(int i = topLine - 1; i < numberOfLines; i++)
            // {
            //     mvwprintw(editor, 1, 1, "%*d  %s", 3,  i + topLine, lines[i + topLine - 1].c_str());
            // }
            // printToLine = numberOfLines < winMaxY ? numberOfLines : winMaxY;
            // printToLine = bottomLine - (topLine - 2);
            printToLine = bottomLine - (topLine - 1);
            for(int i = TOP_SPACING; i < printToLine + TOP_SPACING; i++)
            {
                line_number = topLine + (i - TOP_SPACING);
                mvwprintw(editor, i, 1, "%*d  %s", 3,  line_number, lines[line_number - 1].c_str());
            }
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
                    bottomLine--;
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
            if(Screen.currentLine < bottomLine || (bottomLine - topLine) + 1 != winMaxY)
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
                    bottomLine++;
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

            // reprintVisible(editor, lines);



            // pasting old code and changing it
            // Note to self. Maybe rewrite the whole damn thing. AHHHHHHHHHHHHHHHH
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
            if(bottomLine < editorMaxY)
            {
                bottomLine++;
            }
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
                bottomLine++;
            }
            //wmove(editor, Screen.curY, LEFT_SPACING);
            // if(substrAfterEnter != "")
            //     wclrtoeol(editor); // these two lines are causing a bug if you hit enter first after opening a file/new file
            // if(numberOfLines > currentLine)
            //     wclrtobot(editor);
            // wclrtoeol(editor); // these two lines are causing a bug if you hit enter first after opening a file/new file
            // wclrtobot(editor);
            // Screen.printToLine = Screen.numberOfLines < Screen.winMaxY ? Screen.numberOfLines : Screen.winMaxY;
            // for(int i = Screen.curY - 1; i < Screen.printToLine; i++)
            // {
            //     mvwprintw(editor, i, 1, "%*d  %s", 3,  i + topLine, lines[i + topLine - 1].c_str());
            // }
            reprintVisible(editor, lines);
            wrefresh(editor);
            // currentChar = lines[currentLine - 1].length() + 1;
            wmove(editor, Screen.currentLine + TOP_SPACING - Screen.topLine, Screen.currentChar + LEFT_SPACING - 1);
        }

        int keyCharPrint(WINDOW * editor, Screen &Screen, std::vector<std::string> &lines, std::string &line, std::string &substringUpToEnter, std::string &substrAfterEnter, int &c)
        {
            // Temporary measure to prevent typing offscreen until true left-right scrolling is implemented
            getmaxyx(editor, Screen.winMaxY, Screen.winMaxX);
            if(currentChar == Screen.winMaxX - LEFT_SPACING)
            {
                beep();
                return -1;
            }

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

            return 0;
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
};

#endif
