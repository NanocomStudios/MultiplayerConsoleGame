#include <iostream>
#include <windows.h>
#include <vector>
#include <time.h>

#include "screenCtrl.h"
#include "game.h"

using namespace std;

#define PlayerMoveSpeed 20
#define bottom 24

void Bullet::draw(short xIn, short yIn) {
    x = xIn;
    y = yIn;

    moveCsr(y, x - 1);
    consoleColorSet(96);
    cout << "*";
    consoleColorSet(37);

    previousClock = clock();
}
bool Bullet::animate(int id) {
    /*if (bulletCollision(id)) {
        state = 1;
    }*/

    moveCsr(y, x - 1);
    cout << " ";

    if (state == 0) {

        moveCsr(y + direction, x - 1);
        consoleColorSet(96);
        cout << "*";
        consoleColorSet(37);
        y = y + direction;

        if (y == 2) {
            moveCsr(y, x - 1);
            cout << " ";
            state = 1;
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

void Player::draw(int pos) {
    consoleColorSet(92);
    moveCsr(bottom - 2, position - 4);
    cout << "       ";
    moveCsr(bottom - 2, pos - 1);
    cout << "=";

    moveCsr(bottom - 1, position - 4);
    cout << "       ";
    moveCsr(bottom - 1, pos - 3);
    cout << "=/ \\=";

    moveCsr(bottom, position - 4);
    cout << "       ";
    moveCsr(bottom, pos - 4);
    cout << "|==X==|";
    consoleColorSet(37);
    position = pos;
}

void Opponent::draw(int pos) {
    consoleColorSet(92);
    moveCsr(4, position - 4);
    cout << "       ";
    moveCsr(4, pos - 1);
    cout << "=";

    moveCsr(3, position - 4);
    cout << "       ";
    moveCsr(3, pos - 3);
    cout << "=\\ /=";

    moveCsr(2, position - 4);
    cout << "       ";
    moveCsr(2, pos - 4);
    cout << "|==X==|";
    consoleColorSet(37);
    position = pos;
}
