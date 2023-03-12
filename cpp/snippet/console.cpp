// some useful functions for console

struct console_size
{
    int rows;
    int cols;
};

#ifdef _WIN32
#include <conio.h> // getch
#include <windows.h>

console_size get_console_size()
{
    console_size s;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    s.cols = csbi.dwSize.X;
    s.rows = csbi.dwSize.Y;
    return s;
}

void clean_console() { system("cls"); }

void set_cursor_pos(int x, int y)
{
    COORD pos = {(short)x, (short)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

#endif

#ifdef __linux__

#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

int getch(void)
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

console_size get_console_size()
{
    struct winsize size;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
    console_size s;
    s.cols = size.ws_col;
    s.rows = size.ws_row;
    return s;
}

void clean_console() { printf("\033[2J"); }

void set_cursor_pos(int x, int y) { printf("\033[%d;%dH", x + 1, y + 1); }

#endif
