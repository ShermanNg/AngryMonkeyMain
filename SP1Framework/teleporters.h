#pragma once
#ifndef _TELE_H
#define _TELE_H
#include "Framework\console.h"
#include "Framework\timer.h"
#include <iostream>
#include"barrel.h"
#include"enemy.h"

struct powercoord{
	COORD powerlocation;
};

extern powercoord teleporter1location, teleporter2location, lifepoweruplocation, firepoweruplocation, flameslocation, freezepoweruplocation;

struct powerups{
	bool present;
	bool activated;
	double timestamp;
	bool stamptime;
};

extern powerups life, fire, freeze, tele, flames, frozen;



//Powerups & Teleporters functions
void drawtele();
void drawlife();
void drawfire();
void drawflame();
void drawfreeze();
void drawlifestatus();
void drawfirestatus();
void firepowerup();
void setpowerups(powerups& spawning,bool present);
void resetpowerup();
void initialisepowercoord();
void teleporters();				//teleporters
void extralifepowerup();	   //life power up
void freezepowerup();			//freeze power up
#endif // _TELE_H