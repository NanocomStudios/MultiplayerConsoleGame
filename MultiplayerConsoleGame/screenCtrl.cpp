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

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>


void drawMenu() {
    consoleColorSet(94);
    drawPlayField();
    consoleColorSet(92);
    moveCsr(2, 5);
    cout << "/===   |====\\      /=\\      /====  /====";
    moveCsr(3, 5);
    cout << "|      |    |     /   \\     |      |";
    moveCsr(4, 5);
    cout << "\\===\\  |====/    /=====\\    |      |===";
    moveCsr(5, 5);
    cout << "    |  |        /       \\   |      |";
    moveCsr(6, 5);
    cout << " ===/  |       /         \\  \\====  \\====";

    moveCsr(7, 20);
    cout << "===== |\\   | \\      /    /=\\     |===\\  |===  |==\\  /===";
    moveCsr(8, 20);
    cout << "  |   | \\  |  \\    /    /   \\    |   |  |__   |==/  \\___";
    moveCsr(9, 20);
    cout << "  |   |  \\ |   \\  /    /=====\\   |   |  |     | \\       \\";
    moveCsr(10, 20);
    cout << "===== |   \\|    \\/    /       \\  |===/  |===  |  \\   ===/";
    consoleColorSet(37);
}

void drawPlayField() {
    for (int i = 0; i < 80; i++) {
        cout << "-";
    }
    cout << endl;
    for (int i = 0; i < 23; i++) {
        cout << "|";
        for (int j = 0; j < 78; j++) {
            cout << " ";
        }
        cout << "|" << endl;
    }
    for (int i = 0; i < 80; i++) {
        cout << "-";
    }
}

void normalColor() {
    consoleColorSet(40);
    consoleColorSet(37);
}

void invertColor() {
    consoleColorSet(43);
    consoleColorSet(30);
}

void drawHostGame(short menuSelection) {
    if (menuSelection == 0) {
        invertColor();
    }
    else {
        normalColor();
    }
    moveCsr(14, 32);
    cout << "   Host Game   ";
}

void drawConnect(short menuSelection) {
    if (menuSelection == 1) {
        invertColor();
    }
    else {
        normalColor();
    }
    moveCsr(17, 32);
    cout << "    Connect    ";
}

void drawExit(short menuSelection) {
    if (menuSelection >= 2) {
        invertColor();
    }
    else {
        normalColor();
    }
    moveCsr(20, 32);
    cout << "     Exit!     ";
}

void drawMsgBox() {
    consoleColorSet(104);
    moveCsr(10, 20);
    cout << "==============================";
    moveCsr(11, 20);
    cout << "|                            |";
    moveCsr(12, 20);
    cout << "|                            |";
    moveCsr(13, 20);
    cout << "|                            |";
    moveCsr(14, 20);
    cout << "|                            |";
    moveCsr(15, 20);
    cout << "==============================";
    consoleColorSet(40);
}