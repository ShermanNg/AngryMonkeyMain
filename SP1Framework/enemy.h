#pragma once
#ifndef _ENEMY_H
#define _ENEMY_H
#include "Framework\console.h"
#include <iostream>
#include "barrel.h"
#include <iostream>
#define enemies 8 //Size of enemies
#define sizeX 8 //Size of X Coord Array
#define sizeY 8 //Size of Y Coord Array
#define WIDTH 80
#define HEIGHT 30



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
extern Enemy enemyList[enemies];	//Enemy count
extern bool enemyType[enemies];
extern int enemyX[sizeX];
extern int enemyY[sizeY];

//Enemy functions
void initialiseEnemy();										  // initialise All enemies properties
void drawenemy();
void spawnenemy(double dt);
void drawdead(int &enemy);
void moveEnemy(int &enemy, double dt);								 // all enemy movements
void enemyClimb(int &enemy);								// Enemy Climbing
void climbAlign(int &enemy);							   // Align enemy climb
bool enemyAlive(int &enemy);							  // Enemy death and needs respawn
int rollDice();											 // RNG
bool climbCheck(int &enemy);	//Check if AI can climb
#endif // _ENEMY_H