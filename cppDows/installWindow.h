#pragma once
#include "basicUtilities.h"


void printLine(int n) {
	for (int j = 0; j < consoleHorizontal; j++) {
		printPixel(j, n, BLUE);
	}
	
}
void namePage() {
	vector<thread> printer;
	for (int i = 0; i < consoleVertical; i++) {
		printer.push_back(thread(printLine, i));
	}
	for (int i = 0; i < consoleVertical; i++) {
		printer[i].join();
	}
}