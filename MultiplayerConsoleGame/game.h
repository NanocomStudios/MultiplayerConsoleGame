#pragma once
#ifndef GAME
#define GAME

#define PlayerFireRate 200
#define PlayerMoveSpeed 20

class Bullet {
public:
    short x;
    short y;
    short damage = 1;
    short state = 0;
    short direction = -1;
    clock_t previousClock;

    void draw(short xIn, short yIn);
    bool animate(int id);

};

class Player {
public:
    short position = 40;


public:
    //void moveRight();

    void draw(int pos);

};

class Opponent {
public:
    short position = 40;


public:
    //void moveRight();

    void draw(int pos);

};

#endif GAME