#pragma once
#ifndef _BARREL_H
#define _BARREL_H
#include "Framework\console.h"
#include "enemy.h"
#include <iostream>
#include <string>
#include <sstream>
#define barrelNum 12 // num of barrel allowed at one time on screen
#define barrelSpeed 1

using std::string;


//Struct of Barrel properties
struct Barrel
{
	bool active;
	COORD position;
};

extern Barrel barrellist[barrelNum];
extern Barrel banana[3];

//Barrel functions
void intialisebarrel(void);
void initialisebanana(void);
void drawbarrel();			  // draws out barrel
void drawbanana();			 //  draw banana
void bananaCount();
void barrelshooting(COORD);  // set barrel location for shooting
void Updatebarrel();		// update barrel COORD
#endif // _BARREL_H