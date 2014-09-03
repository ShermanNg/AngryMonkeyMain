#include "menu.h"
#pragma once

int modeSelected = 0;		//Mode selection
int pointed = 0;			//Option of menu selected
bool gameStarted;			//Game started check

extern COORD charLocation;	//Resetting barrel coord
extern bool about;					//Displays instructions

//function for main menu and to check if game started from decision
bool gameStart()
{
	
	//Reinitialize upon menu
	gameStarted = false;	//Game is not started
	editor = false;			//Editor is not started
	versus= false;			//Versus is not started

	//Loop to detect user input and move accordingly until action is selected
	if (isKeyPressed(VK_UP))
	{
		pointed--;					//Moves pointed upwards
		//playGameSound(S_BEEP);		//Sound for scrolling
		if (pointed == -1)			//If pointed exceeds limit, pointed returns to bottom
		{
			pointed = 4;
		}
	}	
	if (isKeyPressed(VK_DOWN))		
	{
		pointed++;					//Moves pointed upwards
		//playGameSound(S_BEEP);		//Sound for scrolling
		if (pointed == 5)			//If pointed exceeds limit, pointed returns to bottom
		{
			pointed = 0;
		}
	}

	//Execute action selected
	if (isKeyPressed(VK_RETURN))
	{
		playGameSound(S_SELECT);	//Sound for selecting option
		modeSelected = pointed;		//Sets mode selected to pointed option
		switch(pointed)
		{
			//Starts Single-Player
		case STARTGAME:
			{
				versus = false;
				gameStarted = true;
				return gameStarted;
				break;
			}
			//Starts Multi-Player
		case STARTMULTI:
			{
				versus = true;
				gameStarted = true;
				return gameStarted;
				break;
			}
			//Starts Level Editor
		case STARTEDITOR:
			{
				versus = false;
				editor = true;
				gameStarted = true;
				LoadMap();
				return gameStarted;
				break;
			}
			//Shows info about game
		case ABOUT:
			{
				gameStarted = false;
				drawInstructions();
				return gameStarted;
				break;
			}
			//Quits game
		case QUITGAME:
			{
				g_quitGame = true;
				return gameStarted;
				break;
			}
			//Game is not started by default
		default:
			{
				return gameStarted;
				break;
			}
		}
	}

	//Resetting barrel location
	for(int i = 0; i<barrelNum; i++)
	{
		barrellist[i].active = false;
		barrellist[i].position.X = charLocation.X;
		barrellist[i].position.Y = charLocation.Y+1;
	}
	return gameStarted;	//Returns T/F based on choices
}

//Draws menu of game
void drawMenu()
{
	string Menu[5] = {"Start Angry Monkeys!", "2-Player Versus Mode","Level Editor", "About Angry Monkeys", "Quit Angry Monkeys"};	//Menu option text
	std::ifstream menuText;
	string menuBanner;

	//clears all the buffer for menu
	clearBuffer(0x0F);

	//Banner
	int y = 0;
	menuText.open("Text/Banner.txt");		//Opens banner text file
	while(!menuText.eof())
	{
		COORD menu = {0,y};
		getline(menuText, menuBanner);		//Stores data to string
		writeToBuffer(menu,menuBanner,0x0F);
		y++;								//Next Line
	}
	menuText.close();						//Close banner text file

	//Option highlighting indicator
	for (int i = 0; i < 5; ++i)
	{
		COORD options = {0,y};		//Option Text
		COORD arrow = {21,y};		//Option Arrow
		if (i == pointed)
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

		y++;			//Increases drawing to next line after finishing each line
	}
}

//Draws instruction of game
void drawInstructions()
{
	std::ifstream instruction;
	string display;

	//Clear buffer for instructions
	clearBuffer(0x0F);

	//Instructions
	int y = 0;
	instruction.open("Text/about.txt");		//Opens about file
	while(!instruction.eof())
	{
		COORD info={0,y};
		getline(instruction, display);		//Stores data to string
		writeToBuffer(info,display,0x0F);
		y++;								//Next Line
	}
	instruction.close();					//Closes about file
	about = true;
	pause = true;							//Pause state
	
}