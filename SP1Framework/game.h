# pragma once
#ifndef _GAME_H
#define _GAME_H
#define WIDTH 80
#define HEIGHT 30
#define endScreenNum 3// No of end screen in game
#define speed 0.3//General speed

#include "Level.h"
#include "HighScore.h"
#include "Framework\timer.h"
#include "Framework\console.h"

#include "menu.h"
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <string>  
#include <sstream>
#include <fstream>

extern char map[WIDTH][HEIGHT];
extern StopWatch g_timer;
extern bool g_quitGame;
extern bool versus;
extern bool editor;

enum Keys
{
	K_LEFT,
	K_RIGHT,
	K_UP,
	K_DOWN,
	K_SPACE,
	K_ESCAPE,
	K_W,
	K_S,
	K_A,
	K_D,
	K_G,
	K_BACKSPACE,
	K_F1,
	K_0,
	K_1,
	K_2,
	K_RETURN,
	K_COUNT,
};

enum SoundType
{
	S_BEEP,
	S_SELECT,
	S_THROW,
	S_HIT,
	S_MUSIC,
	S_COUNT
};

//Main Game functions
void init();					// initialize your variables, allocate memory, etc
void getInput();			   // get input from player
void update(double dt);		  // update the game and the state of the game
void render();               // renders the current state of the game to the console
void shutdown();            // do clean up, free memory
void playGameSound(SoundType sound); // play this sound type

#endif // _GAME_H