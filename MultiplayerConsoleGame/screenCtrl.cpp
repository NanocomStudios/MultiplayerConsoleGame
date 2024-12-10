#include <iostream>

#include "screenCtrl.h"

using namespace std;

void moveCsr(short row, short col) { // https://en.wikipedia.org/wiki/ANSI_escape_code
    char code[] = "e[000;000H";
    code[0] = 27;
    code[2] = 48 + (row / 100);
    code[3] = 48 + ((row % 100) / 10);
    code[4] = 48 + (row % 10);

    code[6] = 48 + (col / 100);
    code[7] = 48 + ((col % 100) / 10);
    code[8] = 48 + (col % 10);

    //printf("%s", code);
    cout << code;
}

void consoleColorSet(int inp) { // inp - https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit

    char code[] = "e[000m";
    code[0] = 27;
    code[2] = 48 + (inp / 100);
    code[3] = 48 + ((inp % 100) / 10);
    code[4] = 48 + (inp % 10);

    //printf("%s", code);
    cout << code;
}

void setCursor(bool state) {
    char code[] = "e[?25l";
    code[0] = 27;

    if (state) {
        code[5] = 'h';
    }
    cout << code;
}