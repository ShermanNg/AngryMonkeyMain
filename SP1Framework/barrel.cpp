#include "barrel.h"
#pragma once

extern COORD charLocation;
extern COORD playerhumanLocation;
Barrel barrellist[barrelNum];
Barrel banana[3];

//initialise barrel
void intialisebarrel(void)
{
	for(int i = 0; i<3; i++)
	{
		barrellist[i].position.X = 0;
		barrellist[i].position.Y= 0;
		barrellist[i].active=false;
	}
}

void initialisebanana(void)
{
	// set the banana Coord and Status
	for(int i = 0; i<3; i++)
	{
		banana[i].active = true;
		banana[i].position.Y = 9;
	}
	banana[0].position.X = 20;
	banana[1].position.X = 40;
	banana[2].position.X = 60;
}

void drawbanana()
{
	for(int i = 0; i<3; i++)
	{
		if(banana[i].active == true)
		{
			gotoXY(banana[i].position);
			colour(0x0E);
			std::cout<<(char)247;
		}
		if(banana[i].active == false)
		{
			gotoXY(banana[i].position);
			colour(0x0E);
			std::cout<<"gone";
		}
	}
}

void barrelshooting(COORD unit)// set barrel according to player's position
{
	for(int i = 0; i<barrelNum; i++)
	{
		if(barrellist[i].active == false)
		{
			barrellist[i].active = true;
			barrellist[i].position.X = unit.X;
			barrellist[i].position.Y = unit.Y;
			break;
		}
	}
}

void drawbarrel()
{
	for(int i = 0; i<barrelNum; i++)
	{
		if(barrellist[i].active == true)
		{
			gotoXY(barrellist[i].position.X+1,barrellist[i].position.Y);
			colour(0xA2);
			std::cout<<(char)4;
		}
	}
}

void Updatebarrel(void)
{
	for(int i = 0; i<barrelNum; i++)
	{
		if (barrellist[i].active == true)
		{
			barrellist[i].position.Y = barrellist[i].position.Y + 2;
			if(barrellist[i].position.Y >= 27)
			{
				barrellist[i].active = false;
				barrellist[i].position.X = charLocation.X;
				barrellist[i].position.Y = charLocation.Y+1;
			}
		}
	}
}