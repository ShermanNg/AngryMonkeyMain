#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"
#include<iostream>
using namespace std;
extern StopWatch g_timer;
extern bool g_quitGame;

enum Keys
{
    K_LEFT,
    K_RIGHT,
	K_SPACE,
    K_ESCAPE,
    K_COUNT,
};

struct barrel
{
	int x;
	int y;
};

void init();                // initialize your variables, allocate memory, etc
void getInput();            // get input from player
void update(double dt);     // update the game and the state of the game
void render();              // renders the current state of the game to the console
void shutdown();            // do clean up, free memory
void DrawMap ();// draws up map
void drawbarrel();
void barrelshooting(COORD);
void Updatebarrel();
void spawnEnemy();// spawns enemy
void moveEnemy();// all enemy movements
int rollDice();// generate random numbers
bool climbCheck(int posX, int posY);//Check if AI can climb

#endif // _GAME_H