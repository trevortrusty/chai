#include <iostream> 
#include <ncurses.h>
#include <vector>

class Box
{
    public:
        int width;
        int height;
        int lineCount;
        int maxLineLength;
        int screenWidth;
        int screenHeight;
        std::vector<std::string> lines;
        Box()
        {
            width = 0;
            height = 0;
            lineCount = 0;
            maxLineLength = 0;
            autoSetPosition();
        }
        void Pack(std::string line)
        {
            if(line.length() % 2 == 0)
                line = line + ' ';
            lines.push_back(line);
            lineCount++;
            if(line.length() > maxLineLength)
                maxLineLength = line.length();


        }
        void autoSetPosition()
        {
            getmaxyx(stdscr, screenHeight, screenWidth);
        }
        void centerX()
        {
            int delta = (maxLineLength + 4) / 2;
            for(int i = 0; i < screenWidth/2 - delta; i++)
            {
                printw(" ");
            }
        }

        void centerY()
        {
            for(int i = 0; i < screenHeight / 2 - height / 2; i++)
            {
                printw("\n");
            }
        }
        void Build()
        {
            int lineLength = 0;
            width = maxLineLength + 4;
            height = lineCount + 4;
            centerY();
            centerX();
            for(int i = 0; i < width + 2; i++)
            {
                printw("*");
            }
            printw("\n");
            centerX();
            printw("*");
            for(int i = 0; i < width; i++)
            {
                printw(" ");
            }
            printw("*\n");
            for(std::string line : lines)
            {
                centerX();
                printw("*");
                for(int i = 0; i < width / 2 - line.length() / 2; i++)
                {
                    printw(" ");
                }
                printw("%s", line.c_str());
                for(int i = 0; i < width / 2 - line.length() / 2; i++)
                {
                    printw(" ");
                }
                printw("*\n");
            }
            centerX();
            printw("*");
            for(int i = 0; i < width; i++)
            {
                printw(" ");
            }
            printw("*\n");
            centerX();
            for(int i = 0; i < width + 2; i++)
            {
                printw("*");
            }
        }
};

int main(int argc, char** argv)
{
    // Initialize the screen
    initscr();

    int height, width;
    Box box;
    box.Pack("Chai");
    box.Pack("Text Editor by Trevor.");
    box.Build();
    getch();

    // deallocates memory and ends ncurses
    endwin();
    return 0;
}