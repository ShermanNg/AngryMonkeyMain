#ifndef _GAME_H
#define _GAME_H
#define WIDTH 80
#define HEIGHT 30
#define barrelNum 3 // num of barrel allowed at one time on screen
#define enemies 8 //Size of enemies
#define sizeX 8 //Size of X Coord Array
#define sizeY 8 //Size of Y Coord Array
#define sizeLadders 8 //Size of Ladders Array
#define endScreenNum 3// No of end screen in game
#define speed 0.3//General speed

# pragma once
#include "Level.h"
#include "HighScore.h"
#include "versusLevel.h"
#include "Framework\timer.h"
#include "Framework\console.h"
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <string>  
#include <sstream>
#include <fstream>
using namespace std;

extern char map[WIDTH][HEIGHT];
extern StopWatch g_timer;
extern bool g_quitGame;
extern bool versus;

enum Keys
{
    K_LEFT,
    K_RIGHT,
	K_SPACE,
    K_ESCAPE,
	K_W,
	K_S,
	K_A,
	K_D,
	K_BACKSPACE,
	K_F1,
	K_RETURN,
    K_COUNT,
};

struct EndScreen
{
	bool active;
	int type;
};

//Main Game functions
void init();					// initialize your variables, allocate memory, etc
void getInput();			   // get input from player
void update(double dt);		  // update the game and the state of the game
void render(int a);               // renders the current state of the game to the console
void shutdown();            // do clean up, free memory
void DrawMap ();		   // draws up map

//Barrel functions
void drawbarrel();			  // draws out barrel
void barrelshooting(COORD);  // set barrel location for shooting
void Updatebarrel();		// update barrel COORD

//Enemy functions
void initialiseEnemy();										  // initialise All enemies properties
void moveEnemy(int &enemy);								 // all enemy movements
void enemyClimb(int &enemy);								// Enemy Climbing
void climbAlign(int &enemy);							   // Align enemy climb
bool enemyAlive(int &enemy);							  // Enemy death and needs respawn
int rollDice();											 // RNG
bool climbCheck(int &enemy);	//Check if AI can climb

//Powerups & Teleporters functions
void teleporters();				//teleporters
void extralifepowerup();	   //life power up

//Game Menu functions
bool gameStart();
void monkeydead();
void showgameover();
void multiplayer1gameover();     
void multiplayer2gameover();
void multiplayerdead();
#endif // _GAME_H