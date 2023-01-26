#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <thread>
#include "basicUtilities.h"
#include "installWindow.h"
#include "console.h"
using namespace std;
using std::thread;
int main() {
    //setConsole();
       

   // thread t1(setScreen);
    //thread t2(mouseInput);
    setScreen();
    
    //t2.join();
   // t1.join();
}