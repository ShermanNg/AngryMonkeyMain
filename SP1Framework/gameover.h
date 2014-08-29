#pragma once
#ifndef _GAMEOVER_H
#define _GAMEOVER_H
#include <iostream>
#include "HighScore.h"
#include "Framework\console.h"
#include "enemy.h"
#include "barrel.h"
#include "game.h"

using std::cout;
using std::endl;

struct EndScreen
{
	bool active;
	int type;
};

struct playerhuman
{
	int health;
};

extern EndScreen Gameover;

//Game Menu functions
void initialiseEnd();
void drawNormalEnd();
void drawplayer1Lose();
void drawplayer2Lose();
void monkeydead();
void showgameover();
void multiplayer1gameover();     
void multiplayer2gameover();
void multiplayerdead();
#endif // _GAMEOVER_H