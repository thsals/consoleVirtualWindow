#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <thread>
#include "basicUtilities.h"
#include "installWindow.h"
#include "console.h"
using namespace std;
using std::thread;
int main() {
    setConsole();

    namePage();

    
    setScreen();
}