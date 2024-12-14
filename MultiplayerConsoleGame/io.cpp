#include <iostream>
#include <windows.h>
#include <conio.h>

#include "screenCtrl.h"
#include "io.h"

using namespace std;

void updateCell(char inputString[] ,int cursorloc, int color) {
	if (cursorloc < 12) {
		if (cursorloc < 3) {
			moveCsr(13, 35 + cursorloc);
			consoleColorSet(color);
			cout << inputString[cursorloc];
		}
		else if (cursorloc < 6) {
			moveCsr(13, 36 + cursorloc);
			consoleColorSet(color);
			cout << inputString[cursorloc];
		}
		else if (cursorloc < 9) {
			moveCsr(13, 37 + cursorloc);
			consoleColorSet(color);
			cout << inputString[cursorloc];
		}
		else {
			moveCsr(13, 38 + cursorloc);
			consoleColorSet(color);
			cout << inputString[cursorloc];
		}
	}
	else if (cursorloc < 17) {
		moveCsr(14, 29 + cursorloc);
		consoleColorSet(color);
		cout << inputString[cursorloc];
	}
}

void getIP(char inputString[]) {
	int cursorloc = 0;
	int cursorline = 13;
	
	system("cls");
	consoleColorSet(94);
	drawPlayField();
	consoleColorSet(37);
	drawMsgBox();

	moveCsr(11,26);
	consoleColorSet(104);
	cout << "   Connect to a host game";
	moveCsr(13,30);
	cout << "IP:  000.000.000.000";
	moveCsr(14, 34);
	cout << "PORT:  27015";
	
	for (int i = 0; i < 12; i++) {
		updateCell(inputString, i, 104);
	}
	updateCell(inputString, cursorloc, 102);
	consoleColorSet(40);
	while (1) {

		char inp = _getch();
		if (inp == 77) {
			if (cursorloc < 16) {
				updateCell(inputString, cursorloc, 104);
				cursorloc++;
				updateCell(inputString, cursorloc, 102);
			}
			
		}else if (inp == 75) {
			if (cursorloc > 0) {
				updateCell(inputString, cursorloc, 104);
				cursorloc--;
				updateCell(inputString, cursorloc, 102);
			}

		}
		else if ((inp >= 48) && (inp <= 57)) {
			if (cursorloc < 17) {
				inputString[cursorloc] = inp;
				updateCell(inputString ,cursorloc, 104);
				cursorloc = (cursorloc < 16) ? cursorloc + 1 : cursorloc;
				updateCell(inputString ,cursorloc, 102);
			}

		}
		else if (inp == 13) {
			break;
		}
	}
	consoleColorSet(40);

}