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

	//Loop to detect user input and move accordingly until action is selected

	if (isKeyPressed(VK_UP))
	{
		pointer--;
		if (pointer == -1)
		{
			pointer = 3;
		}
	}
	if (isKeyPressed(VK_DOWN))
	{
		pointer++;
		if (pointer == 4)
		{
			pointer = 0;
		}
	}
	//Execute action selected
	if (isKeyPressed(VK_SPACE))
	{
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
		}
	}
	for(int i = 0; i<barrelNum; i++)
	{
		barrellist[i].active = false;
		barrellist[i].position.X = charLocation.X;
		barrellist[i].position.Y = charLocation.Y+1;
	}
}

void drawMenu()
{
	//Menu Vars
	string Menu[4] = {"Start Angry Monkeys!", "2-Player Versus Mode", "About Angry Monkeys", "Quit Angry Monkeys."};
	std::ifstream menuText;
	string menuBanner;

	cls();	//Clear screen to update input

	//Banner
	colour(0x0F);
	menuText.open("Text/Banner.txt");
	while(!menuText.eof())
	{
		getline(menuText, menuBanner);
		cout << menuBanner << endl;
	}
	menuText.close();

	//Text Attribute only for Main Menu text
	colour(0x0F);
	cout << "Please navigate through the menu and select your desired action using spacebar" << endl;
	for (int i = 0; i < 4; ++i)
	{
		if (i == pointer)
		{
			//Selected options lights up to indicate selection
			colour(0x0F);
			Beep(840,30);
			cout << Menu[i] << " <-"<< endl;
		}
		else
		{
			//Unselected options are greyed
			colour(0x07);
			cout << Menu[i] << endl;
		}
	}
}

void drawInstructions()
{
	std::ifstream instruction;
	string display;
	cls();	//Clear screen to update input

	//Banner
	colour(0x0F);
	instruction.open("Text/about.txt");
	while(!instruction.eof())
	{
		getline(instruction, display);
		cout << display << endl;
	}
	instruction.close();
	pause = true;
}