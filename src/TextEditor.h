#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <ncurses.h>
#include <vector>
#include <string>

class TextEditor
{
    private:
        int maxY, maxX;         // Max cursor positions for entire terminal (stdscr)
        int winMaxY, winMaxX;   // Max cursor positions for just the editor window
        int curY, curX;
        int numberOfLines = 1;  // Number of lines that have been created/exist by the user
        int currentLine = 1;    // Line the cursor is on within the text editor
        int currentChar = 1;    // Char/space the cursor is on within the line
        int topLine = 1;        // Keeps track of the top visible line when scrolling
        int bottomLine = 1;     // Keeps track of the bottom visible line when scrolling
        std::vector<std::string> lines;
        std::string line;

    public:        
        const int TOP_SPACING = 0;
        const int LEFT_SPACING = 6;
        TextEditor()
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
            curY = curX = 0;
        }

        void pushLine(std::string l)
        {
            lines.push_back(l);
        }

        void pushLine()
        {
            lines.push_back(line);
        }

        void setEmptyLineBuffer()
        {
            line = "";
        }

        void setLineBuffer(std::string s)
        {
            line = s;
        }

        std::string lineAt(int i)
        {
            return lines[i];
        }

        void selectLineByCursor()
        {
            line = lines[getCurrentLinePos() - 1]; // Set the current line string
        }

        std::vector<std::string> getAllLines() const
        {
            return lines;
        }

        int autoSetTermDimensions()
        {
            getmaxyx(stdscr, maxY, maxX);
            return 0;
        }

        int autoSetEditorDimensions(WINDOW *editor)
        {
            getmaxyx(editor, winMaxY, winMaxX);
            return 0;
        }

        int getMaxTermY()
        {
            autoSetTermDimensions();
            return maxY;
        }

        int getMaxTermX()
        {
            autoSetTermDimensions();
            return maxX;
        }

        int getMaxEditorY(WINDOW * editor)
        {
            autoSetEditorDimensions(editor);
            return winMaxY;
        }

        int getMaxEditorX(WINDOW * editor)
        {
            autoSetEditorDimensions(editor);
            return winMaxX;
        }

        int getCurY(WINDOW * editor)
        {
            getyx(editor, curY, curX);
            return curY;
        }

        int getCurX(WINDOW * editor)
        {
            getyx(editor, curY, curX);
            return curX;
        }

        void moveToStart(WINDOW * editor)
        {

        }

        int getNumberOfLines()
        {
            return numberOfLines;
        }

        int getCurrentCharPos()
        {
            return currentChar;
        }

        int getCurrentLinePos()
        {
            return currentLine;
        }

        int getTopLinePos()
        {
            return topLine;
        }

        int getBottomLinePos()
        {
            return bottomLine;
        }

        int setCurrentCharPos(int n)
        {
            currentChar = n;
            return 0;
        }

        int incCurrentCharPos()
        {
            currentChar++;
            return 0;
        }
        
        int incCurrentCharPos(int n)
        {
            currentChar += n;
            return 0;
        }

        int decCurrentCharPos()
        {
            currentChar--;
            return 0;
        }

        int decCurrentCharPos(int n)
        {
            currentChar -= n;
            return 0;
        }

        int setCurrentLinePos(int n)
        {
            currentLine = n;
            return 0;
        }

        int incCurrentLinePos()
        {
            currentLine++;
        }

        int decCurrentLinePos()
        {
            currentLine--;
        }

        int setNumberOfLines(int n)
        {
            numberOfLines = n;
            return 0;
        }

        int incNumberOfLines()
        {
            numberOfLines++;
            return 0;
        }

        int decNumberOfLines()
        {
            numberOfLines--;
            return 0;
        }

        int incTopLinePos()
        {
            topLine++;
            return 0;
        }

        int decTopLinePos()
        {
            topLine--;
            return 0;
        }

        int setTopLinePos(int n)
        {
            topLine = n;
            return 0;
        }

        int incBottomLinePos()
        {
            bottomLine++;
            return 0;
        }

        int decBotomLinePos()
        {
            bottomLine--;
            return 0;
        }

        int setBottomLinePos(int n)
        {
            bottomLine = n;
            return 0;
        }

        void reprintVisible(WINDOW * editor)
        {
            wclear(editor);
            int line_number;
            int printToLine = getBottomLinePos() - (getTopLinePos() - 1);
            for(int i = TOP_SPACING; i < printToLine + TOP_SPACING; i++)
            {
                line_number = topLine + (i - TOP_SPACING);
                mvwprintw(editor, i, 1, "%*d  %s", 3,  line_number, lines[line_number - 1].c_str());
            }
        }

        void keyUp(WINDOW * editor)
        {
            lines[getCurrentLinePos() - 1] = line;
            if(getCurrentLinePos() > getTopLinePos())
            {
                if(getCurrentLinePos() > 1)
                {
                    decCurrentLinePos();
                    if(getCurrentCharPos() > lines[getCurrentLinePos() - 1].length())
                    {
                        setCurrentCharPos(lines[getCurrentLinePos() - 1].length() + 1);
                        wmove(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), lines[getCurrentLinePos() - 1].length() + LEFT_SPACING);
                    }
                    else {
                        wmove(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), getCurrentCharPos() + LEFT_SPACING - 1);
                    }
                    line = lines[getCurrentLinePos() - 1];
                }
                else
                {
                    // Can't move up any farther, move cursor to beginning of current line
                    setCurrentCharPos(1);
                    wmove(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), LEFT_SPACING);
                }
            }
            else {
                if(currentLine > 1)
                {
                    decTopLinePos();
                    wscrl(editor, -1); // Scroll the window up by one line
                    decCurrentLinePos();

                    // After scrolling up, reprint line number and the text belonging to that line
                    mvwprintw(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), 1, "%*d  %s", 3,  getCurrentLinePos(), lines[getCurrentLinePos() - 1].c_str());

                    // Check if the previous line up is shorter than the current line. 
                    // If it is shorter, cursor moves to end of the previous line in order to stay in bounds
                    if(getCurrentCharPos() > lines[getCurrentLinePos() - 1].length())
                    {
                        // Screen.currentChar = lines[Screen.currentLine - 1].length() + 1;
                        setCurrentCharPos(lines[getCurrentLinePos() - 1].length() + 1);
                        wmove(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), lines[getCurrentLinePos() - 1].length() + LEFT_SPACING);
                    }
                    else {
                        wmove(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), getCurrentCharPos() + LEFT_SPACING - 1);
                    }
                    line = lines[getCurrentLinePos() - 1];
                    decBotomLinePos();
                }
                else
                {
                    // Can't move up any farther, move cursor to beginning of current line
                    setCurrentCharPos(1);
                    wmove(editor, TOP_SPACING, LEFT_SPACING);
                }
            }
        }

        void keyDown(WINDOW * editor)
        {
            lines[getCurrentLinePos() - 1] = line;
            if(getCurrentLinePos() < getBottomLinePos() || (getBottomLinePos() - getTopLinePos() + 1 != getMaxEditorY(editor)))
            {
                if(getCurrentLinePos() != getNumberOfLines())
                {
                    incCurrentLinePos();
                    // Check if the next line down is shorter than the current line. 
                    // If it is shorter, cursor moves to end of the next line in order to stay in bounds
                    if(getCurrentCharPos() > lines[getCurrentLinePos() - 1].length())
                    {                        
                        setCurrentCharPos(lines[getCurrentLinePos() - 1].length() + 1);
                        wmove(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), lines[getCurrentLinePos() - 1].length() + LEFT_SPACING);
                    }
                    else {
                        wmove(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), getCurrentCharPos() + LEFT_SPACING - 1);
                    }
                    line = lines[getCurrentLinePos() - 1];
                }
                else
                {
                    // Can't move down any farther, move cursor to beginning of current line
                    setCurrentCharPos(1);
                    wmove(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), LEFT_SPACING);
                }
            }
            else {
                if(getCurrentLinePos() != getNumberOfLines())
                {
                    incTopLinePos();
                    wscrl(editor, 1); // Scroll the window up by one line
                    incCurrentLinePos();
                    mvwprintw(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), 1, "%*d  %s", 3,  getCurrentLinePos(), lines[getCurrentLinePos() - 1].c_str());

                    // Check if the next line down is shorter than the current line. 
                    // If it is shorter, cursor moves to end of the next line in order to stay in bounds
                    if(currentChar > lines[currentLine - 1].length())
                    {
                        setCurrentCharPos(lines[getCurrentLinePos() - 1].length() + 1);
                        wmove(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), lines[getCurrentLinePos() - 1].length() + LEFT_SPACING);
                    }
                    else {
                        wmove(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), getCurrentCharPos() + LEFT_SPACING - 1);
                    }
                    line = lines[getCurrentLinePos() - 1];
                    incBottomLinePos();
                }
                else
                {
                    // Can't move down any farther, move cursor to beginning of current line
                    setCurrentCharPos(1);
                    wmove(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), LEFT_SPACING);
                }                   
            }
        }

        void keyLeft(WINDOW * editor)
        {
            if(getCurrentCharPos() != 1)
            {
                decCurrentCharPos();
                wmove(editor, getCurrentLinePos() + TOP_SPACING - topLine, getCurrentCharPos() + LEFT_SPACING - 1);
            }
            else {
                wmove(editor, getCurrentLinePos() + TOP_SPACING - topLine, getCurrentCharPos() + LEFT_SPACING - 1);
            }
        }

        void keyRight(WINDOW * editor)
        {
            if(getCurrentCharPos() <= line.length())
            {
                currentChar++;
                wmove(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), getCurrentCharPos() + LEFT_SPACING - 1);
            }
        }

        void keyEnter(WINDOW * editor, std::string &substringUpToEnter, std::string &substrAfterEnter)
        {
            
            // Get cursor position and print line number to the next line
            substringUpToEnter = line.substr(0,getCurrentCharPos() - 1);
            substrAfterEnter = line.substr(getCurrentCharPos() - 1);
            lines[getCurrentLinePos() - 1] = substringUpToEnter;
            line = substrAfterEnter;
            if(getBottomLinePos() < getMaxEditorY(editor))
            {
                incBottomLinePos();
            }
            // Checks if space is available on the visible screen to start new line
            if(getCurrentLinePos() < getMaxEditorY(editor))
            {
                if(getCurrentLinePos() == getNumberOfLines())
                {
                    lines.push_back(line);
                }
                else {
                    lines.insert(lines.begin() + (getCurrentLinePos()), line);
                }
                
                incCurrentLinePos();
                setCurrentCharPos(1);
                incNumberOfLines();
                mvwprintw(editor, getNumberOfLines() + TOP_SPACING - 1, 1, "%*d", 3,  numberOfLines);
                wrefresh(editor);
                wmove(editor, getCurY(editor) + 1, LEFT_SPACING);
            }
            // If no space left in window for a new line, scroll window down.
            else {
                if(getCurrentLinePos() == getNumberOfLines())
                {
                    lines.push_back(line);
                }
                else {
                    lines.insert(lines.begin() + (getCurrentLinePos()), line);
                }
                incCurrentLinePos();
                setCurrentCharPos(1);
                incNumberOfLines();
                incTopLinePos();            // update which line is seen at the top of the window
                wscrl(editor, 1);           // Scroll the window down by one line
                wrefresh(editor);
                wmove(editor, getCurY(editor), LEFT_SPACING);
                incBottomLinePos();
            }
            reprintVisible(editor);
            wrefresh(editor);
            wmove(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), getCurrentCharPos() + LEFT_SPACING - 1);
        }

        int keyCharPrint(WINDOW * editor, std::string &substringUpToEnter, std::string &substrAfterEnter, int &c)
        {
            // Temporary measure to prevent typing offscreen until true left-right scrolling is implemented
            if(getCurrentCharPos() == getMaxEditorX(editor) - LEFT_SPACING)
            {
                beep();
                return -1;
            }
            line.insert(line.begin() + getCurrentCharPos() - 1, (char)c);
            incCurrentCharPos();
            winsch(editor, (char)c);
            wrefresh(editor);
            wmove(editor, getCurY(editor), getCurX(editor) + 1);
            lines[getCurrentLinePos() - 1] = line;

            return 0;
        }

        void keyBackspace(WINDOW * editor)
        {
            if(currentChar > 1)
            {
                currentChar--;
                line.erase(currentChar - 1, 1);
                lines[currentLine - 1].erase(currentChar - 1, 1);
                wmove(editor, currentLine + TOP_SPACING - topLine, LEFT_SPACING);
                wclrtoeol(editor);

                mvwprintw(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), 1, "%*d  %s", 3,  getCurrentLinePos(), lines[getCurrentLinePos() - 1].c_str());
                wrefresh(editor);
                
                wmove(editor, getCurrentLinePos() + TOP_SPACING - getTopLinePos(), getCurrentCharPos() + LEFT_SPACING - 1);
            }
            else {
                beep();
            }
        }

        void keyTab(WINDOW * editor)
        {
            line.insert(line.begin() + getCurrentCharPos() - 1, 4, ' '); // 4 Spaces
            incCurrentCharPos(4);
            for(int i = 0; i < 4; ++i)
            {
                winsch(editor, ' ');
            }
            wrefresh(editor);
            wmove(editor, getCurY(editor), getCurX(editor) + 4);
            lines[getCurrentLinePos() - 1] = line;
        }
};

#endif