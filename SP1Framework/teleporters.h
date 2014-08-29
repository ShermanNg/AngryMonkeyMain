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

struct activatepowerup{
	bool activated;
};

extern activatepowerup afire;

struct randomlevelpos{
	int levelpos;
};

extern randomlevelpos lfire;

struct timestamps{
	double timestamp;
	bool stamptime;
};


struct powerups{
	bool present;
};

extern powerups plife, pfire, pfreeze;

extern timestamps tele, life, fire, flame, freeze;

//Powerups & Teleporters functions
void drawtele();
void drawlife();
void drawfire();
void drawflame();
void firepowerup();
void setpowerups(powerups& spawning,bool present);
void settimeStamps(timestamps& timing,double timestamp, bool stamptime);
void activatepowerups(activatepowerup& state,bool activated);
void setrandomlevelpos(randomlevelpos& random,int levelpos);
void setrandomlevelpos(randomlevelpos& random,int levelpos);
void initialisepowercoord();
void teleporters();				//teleporters
void extralifepowerup();	   //life power up
#endif // _TELE_H