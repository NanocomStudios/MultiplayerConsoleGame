#include <iostream>
#include <Windows.h>
#include <conio.h>

#include "server.h"
#include "client.h"
#include "screenCtrl.h"

using namespace std;

short menuSelection = 0;

int main(int argc, char** argv) {
    setCursor(false);
    system("cls");

    drawMenu();

    drawHostGame(menuSelection);
    drawConnect(menuSelection);
    drawExit(menuSelection);

    int inp;

    while (1) {
        inp = _getch();

        //77 - right
        //75 - left
        //72 - up
        //80 - down

        if (inp == 72) {
            if (menuSelection > 0) {
                menuSelection--;
            }
        }
        else if (inp == 80) {
            if (menuSelection < 2) {
                menuSelection++;
            }
        }
        else if ((inp == 13) || (inp == 32)) {
            if (menuSelection == 0) {
                server();
            }else if (menuSelection == 1) {
                client();
            }
            else if (menuSelection >= 2) {
                normalColor();
                system("cls");
                return 0;
            }
        }

        drawHostGame(menuSelection);
        drawConnect(menuSelection);
        drawExit(menuSelection);

    }

}