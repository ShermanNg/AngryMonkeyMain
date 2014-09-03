#pragma once
#ifndef _ENEMY_H
#define _ENEMY_H

#include "Framework\console.h"
#include <iostream>
#include "barrel.h"
#include "game.h"

#define maxEnemies 8	//Size of enemies
#define sizeX 8			//Size of X Coord Array
#define sizeY 8			//Size of Y Coord Array
#define WIDTH 80		//Width of map
#define HEIGHT 30		//Height of map

//Struct of enemy properties
struct Enemy
{
	int enemy;
	int health;
	int randNum;
	int dodgeChance;
	bool toRight;
	bool canClimb;
	bool isClimbing;
	bool canMove;
	bool isAlive;
	bool isJumpingOff;
	bool isSpecial;
	double respawnTimer;
	COORD position;
};

//Using defined array size
extern Enemy enemyList[maxEnemies];			//Enemy count
extern bool enemyType[maxEnemies];			//Types of enemies
extern int enemyX[sizeX];					//X Coord of default spawn point
extern int enemyY[sizeY];					//Y Coord of default spawn point

//Enemy functions
int rollDice();								//RNG

void initialiseEnemy();						//Initialise All enemies properties
void activateEnemy(int enemyCount);			//Activates initialised enemies
void moveEnemy(int &enemy);					//All enemy movements
void enemyClimb(int &enemy);				//Enemy Climbing
void climbAlign(int &enemy);				//Align enemy climb
bool climbCheck(int &enemy);				//Check if AI can climb
bool enemyAlive(int &enemy);				//Enemy death and needs respawn

void drawEnemy(int enemyCount);				//Draws enemies
void drawDeadEnemy(int &enemy);				//Draws dead enemies
void showKill();							//Draw KillCount
#endif // _ENEMY_H