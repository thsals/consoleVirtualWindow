#pragma once
#include <Windows.h>
#include <cwchar>
#include <thread>
#include <vector>
#define consoleHorizontal 700
#define consoleVertical 188
using std::thread;
using std::vector;
HANDLE COUT = 0;
HANDLE CIN = 0;
int x;
int y;


enum Color {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};

void changeFont(int n) {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = n;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void setConsole() {
    changeFont(6);
    system("mode con COLS=700");
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);

    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    ConsoleCursor.bVisible = false;
    ConsoleCursor.dwSize = 1;

    SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

int be_input() {
    INPUT_RECORD input_record;
    DWORD input_count;

    PeekConsoleInput(CIN, &input_record, 1, &input_count);
    return input_count;
}

int get_input(WORD* vkey, COORD* pos) {
    INPUT_RECORD input_record;
    DWORD input_count;

    ReadConsoleInput(CIN, &input_record, 1, &input_count);
    switch (input_record.EventType) {
    case MOUSE_EVENT:
        if (pos && (input_record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)) {
            CONSOLE_SCREEN_BUFFER_INFO csbi;

            GetConsoleScreenBufferInfo(COUT, &csbi);

            *pos = input_record.Event.MouseEvent.dwMousePosition;
            pos->X -= csbi.srWindow.Left;
            pos->Y -= csbi.srWindow.Top;

            return MOUSE_EVENT;
        }
        break;
    }
    return 0;
}

void gotoxy(int x, int y) {
    COORD Cur;
    Cur.X = x;
    Cur.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void mouseInput() {
    DWORD mode;
    WORD key;
    COORD pos;

    int event;

    CIN = GetStdHandle(STD_INPUT_HANDLE);
    COUT = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(CIN, &mode);
    SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);

    while (1) {
        if (be_input()) {
            if (get_input(&key, &pos) != 0) {
                MOUSE_EVENT;
                x = pos.X;
                y = pos.Y;
                gotoxy(x, y);
                printf("%d %d", x, y);
            }
        }
    }
}

void printPixel(int x,int y,int color) {

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color * 16 + color);
    gotoxy(x, y);
    printf(" ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 * 16 + 0);
}