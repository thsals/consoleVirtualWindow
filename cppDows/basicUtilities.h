#pragma once
#include <Windows.h>
HANDLE COUT = 0;    // 콘솔 출력 장치
HANDLE CIN = 0;        // 콘솔 입력 장치

void setConsole() {
    system("mode con COLS=700");
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
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