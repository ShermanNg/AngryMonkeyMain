#pragma once
#ifndef _BARREL_H
#define _BARREL_H
#include "Framework\console.h"
#include <iostream>
#define barrelNum 3 // num of barrel allowed at one time on screen
#define barrelSpeed 2


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
void barrelshooting(COORD);  // set barrel location for shooting
void Updatebarrel(double dt);		// update barrel COORD
#endif // _BARREL_H