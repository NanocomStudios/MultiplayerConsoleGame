#pragma once
#ifndef SCREEN_CTRL
#define SCREEN_CTRL

void moveCsr(short row, short col);
void consoleColorSet(int inp);
void setCursor(bool state);

void drawPlayField();
void drawMenu();
void drawHostGame(short menuSelection);
void drawConnect(short menuSelection);
void drawExit(short menuSelection);
void drawMsgBox();

void normalColor();
void invertColor();
#endif