#include <iostream>
#include "basicUtilities.h"
using namespace std;

int main() {
    setConsole();

    DWORD mode;
    WORD key;
    COORD pos;

    int event;
    int x;
    int y;

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