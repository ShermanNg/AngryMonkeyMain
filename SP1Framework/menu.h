#pragma once
#ifndef MENU_H
#define MENU_H

#include "game.h"
#include"barrel.h"

using std::string;
using std::cout;
using std::endl;

extern bool versus;
extern bool g_quitGame;
extern bool pause;

// Enum of Game State
enum States
{
	STARTGAME = 0,
	STARTMULTI,
		STARTEDITOR,
	ABOUT,
	QUITGAME
};

bool gameStart();
void drawMenu();
void drawInstructions();
#endif	//MENU_H