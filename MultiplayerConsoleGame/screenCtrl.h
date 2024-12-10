#pragma once
#ifndef SCREEN_CTRL
#define SCREEN_CTRL

void moveCsr(short row, short col);
void consoleColorSet(int inp);
void setCursor(bool state);
#endif