#include "menu.h"
#pragma once

int modeSelected = 0;	
int pointer = 0;
bool gameStarted;
bool display;

extern COORD charLocation;

//function for main menu and to check if game started from decision
bool gameStart()
{
	//Reinitialize upon menu
	gameStarted = false;
	editor = false;
	versus= false;

	//Loop to detect user input and move accordingly until action is selected

	if (isKeyPressed(VK_UP))
	{
		pointer--;
		playGameSound(S_BEEP);
		if (pointer == -1)
		{
			pointer = 4;
		}
	}
	if (isKeyPressed(VK_DOWN))
	{
		pointer++;
			playGameSound(S_BEEP);
		if (pointer == 5)
		{
			pointer = 0;
		}
	}
	//Execute action selected
	if (isKeyPressed(VK_RETURN))
	{
			playGameSound(S_SELECT);
		modeSelected = pointer;
		switch(pointer)
		{
		case STARTGAME:
			{
				versus = false;
				gameStarted = true;
				return gameStarted;
				break;
			}
		case STARTMULTI:
			{
				versus = true;
				gameStarted = true;
				return gameStarted;
				break;
			}
			case STARTEDITOR:
			{
				
				versus = false;
						editor = true;
				gameStarted = true;
				LoadMap();
				return gameStarted;
				break;
			}
		case ABOUT:
			{
				gameStarted = false;
				drawInstructions();
				return gameStarted;
				break;
			}
		case QUITGAME:
			{
				g_quitGame = true;
				return gameStarted;
				break;
			}
		default:
			{
				return gameStarted;
				break;
			}
		}

	}
	for(int i = 0; i<barrelNum; i++)
	{
		barrellist[i].active = false;
		barrellist[i].position.X = charLocation.X;
		barrellist[i].position.Y = charLocation.Y+1;
	}
	return gameStarted;
}

void drawMenu()
{
	//Menu Vars
	string Menu[5] = {"Start Angry Monkeys!", "2-Player Versus Mode","Level Editor", "About Angry Monkeys", "Quit Angry Monkeys."};
	std::ifstream menuText;
	string menuBanner;

	//clears all the buffer for menu
	clearBuffer(0x0F);

	//Banner
	int y = 0;
	menuText.open("Text/Banner.txt");
	while(!menuText.eof())
	{
		COORD menu = {0,y};
		getline(menuText, menuBanner);
		writeToBuffer(menu,menuBanner,0x0F);
		y++;
		
	}
	menuText.close();

	//Text Attribute only for Main Menu text

	for (int i = 0; i < 5; ++i)
	{
		COORD options = {0,y};
		COORD arrow = {21,y};
		if (i == pointer)
		{
			//Selected options lights up to indicate selection
			writeToBuffer(options,Menu[i],0x0F);
			writeToBuffer(arrow,"<-",0x0F);
		}
		else
		{
			//Unselected options are greyed
			writeToBuffer(options,Menu[i],0x07);
			writeToBuffer(arrow,"  ",0x0F);
		}
		
		y++;
	}
}

void drawInstructions()
{
	std::ifstream instruction;
	string display;
	
	//clear buffer for instructions
	clearBuffer(0x0F);

	//Banner
	int y = 0;
	instruction.open("Text/about.txt");
	while(!instruction.eof())
	{
		COORD info={0,y};
		getline(instruction, display);
		writeToBuffer(info,display,0x0F);
		y++;
	}
	instruction.close();
	pause = true;
	
}