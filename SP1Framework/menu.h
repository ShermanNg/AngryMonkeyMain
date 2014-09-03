#pragma once
#ifndef MENU_H
#define MENU_H

#include "game.h"
#include"barrel.h"

using std::string;
using std::cout;
using std::endl;

extern bool versus;			//Versus mode for menu
extern bool g_quitGame;		//Quit game for menu
extern bool pause;			//Pause for instructions

// Enum of Game State
enum States
{
	STARTGAME = 0,
	STARTMULTI,
	STARTEDITOR,
	ABOUT,
	QUITGAME
};

bool gameStart();			//Decides if game starts
void drawMenu();			//Menu drawing
void drawInstructions();	//Instructions drawing
#endif	//MENU_H