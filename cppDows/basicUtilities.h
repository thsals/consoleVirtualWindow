#pragma once
#include <Windows.h>
#include <cwchar>
#include <thread>
#include <vector>
#include <string>
#include "console.h"
#include "images.h"
#define consoleHorizontal 700
#define consoleVertical 188
using std::thread;
using std::vector;
CConsoleScreen ConsoleScreen;
HANDLE COUT = 0;
HANDLE CIN = 0;

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
                int x = pos.X;
                int y = pos.Y;
                //ConsoleScreen.DrawPoint(x, y, RED, RED, ' ');
                gotoxy(x, y);
                
                printf(" ");
            }
        }
    }
}

VOID setScreen( ) {
    ConsoleScreen.Create(Width, Height, "테스트", WHITE, BLACK);
    ConsoleScreen.SetCursorState(FALSE);

    DOUBLE Rad = 0.0f;

    SHORT FontPosX = -30;

    DWORD FpsCount = 0;
    DWORD Fps = 0;
    DWORD NextGetTickCount = GetTickCount();

    DWORD CurrenGetTickCount = GetTickCount();


    DWORD mode;
    WORD key;
    COORD pos;

    int event;

    CIN = GetStdHandle(STD_INPUT_HANDLE);
    COUT = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(CIN, &mode);
    SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);
    while (TRUE) {

        
        for (SHORT X = 0; X <= Width; X++)
        {
            for (SHORT Y = 0; Y <= Height; Y++)
            {   
                ConsoleScreen.DrawPoint(X, Y, D_BLUE, D_BLUE, ' ');
            }
        }
        int isClick = 0;
        char firstString[50] = "이 PC를 누가 사용하나요?";
        ConsoleScreen.DrawFont(40, 3, WHITE, D_BLUE, firstString);
        for (int i = 0; i < strlen(firstString); i++) {
            ConsoleScreen.DrawPoint(40 + i, 23, WHITE, WHITE, ' ');
        }

        for (int i = 0; i < 14; i++) {
            for (int j = 0; j < 15; j++) {
                if (profileIcon[i][j] == 1) {
                    ConsoleScreen.DrawPoint(45 + j, 6 + i, GRAY, GRAY, ' ');
                }
            }
        }

        ConsoleScreen.DrawFont(88, 25, WHITE, BLUE, " 다음 ");

        if (be_input()) {
            if (get_input(&key, &pos) != 0) {
                MOUSE_EVENT;
                int x = pos.X;
                int y = pos.Y;

            }
        }

        ConsoleScreen.Rander();        
    }
    return;
}