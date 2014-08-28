// This is the main file for the game logic and function
//
//

#include "player.h"
#include "teleporters.h"
#include "Level.h"
#include "game.h"
#include "gameover.h"
#pragma once


using std::cin;
using std::cout;
using std::endl;
using std::string;

double elapsedTime;
double deltaTime;
double dodgeTimer;
bool keyPressed[K_COUNT];
bool gameStarted = false;
bool pause;


COORD consoleSize;
COORD charLocation;
COORD playerhumanLocation;

// Enum of Game State
enum States
{
	STARTGAME = 0,
	ABOUT,
	QUITGAME
};

void init()
{
	// Set precision for floating point output
	std::cout << std::fixed << std::setprecision(3);

	SetConsoleTitle(L"Angry Monkey");

	// Sets the console size, this is the biggest so far.
	setConsoleSize(79, 28);

	// Get console width and height
	CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */     

	/* get the number of character cells in the current buffer */ 
	GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );
	consoleSize.X = csbi.srWindow.Right + 1;
	consoleSize.Y = csbi.srWindow.Bottom + 1;

	// set the character to be in the center of the screen.
	charLocation.X = 40;
	charLocation.Y = 1;

	// set 2nd player coord
	playerhumanLocation.X = 40;
	playerhumanLocation.Y = 25;

	// set enemy variables
	initialiseEnemy();

	//initialise endscreen
	initialiseEnd();

	//initialise banana
	initialisebanana();

	Highscoreload();
	elapsedTime = 0.0;
	pause = false;
}

//Rolling dice for chance to climb

void shutdown()
{
	// Reset to white text on black background
	colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void getInput()
{    
	keyPressed[K_LEFT] = isKeyPressed(VK_LEFT);
	keyPressed[K_RIGHT] = isKeyPressed(VK_RIGHT);
	keyPressed[K_SPACE] = isKeyPressed(VK_SPACE);
	keyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
	keyPressed[K_W] = isKeyPressed(0x57);
	keyPressed[K_S] = isKeyPressed(0x53);
	keyPressed[K_A] = isKeyPressed(0x41);
	keyPressed[K_D] = isKeyPressed(0x44);
	keyPressed[K_BACKSPACE] = isKeyPressed(0x08);
	keyPressed[K_RETURN] = isKeyPressed(VK_RETURN);
	keyPressed[K_F1] = isKeyPressed(VK_F1);
}

void update(double dt)
{
	if (pause == false)
	{
		// get the delta time
		elapsedTime += dt;
		deltaTime = dt;
		dodgeTimer += dt;
		int printL = 0;
		int leftNum = 0;
		int rightNum = 0;


		// Updating the location of the character based on the key press
		if (keyPressed[K_LEFT] && charLocation.X > 2)
		{
			Beep(1440, 30);
			charLocation.X-=3;
		}
		if (keyPressed[K_RIGHT] && charLocation.X < consoleSize.X - 4)
		{
			Beep(1440, 30);
			charLocation.X+=3;
		}
		if(keyPressed[K_SPACE])
		{
			int barrelcount = 0;
			if(barrelcount<barrelNum)
			{
				barrelshooting(charLocation);
				barrelcount++;
			}
			if(barrelcount == barrelNum)
			{
				barrelcount = 0;
			}
		}
		// Updating the location of player 2
		if(keyPressed[K_W] && map[playerhumanLocation.X+1][playerhumanLocation.Y+2] == '2')
		{
			Beep(1440, 30);
			playerhumanLocation.Y--;
		}
		if(keyPressed[K_S] && map[playerhumanLocation.X+1][playerhumanLocation.Y+2] == '2' && map[playerhumanLocation.X+1][playerhumanLocation.Y+3] != '1')
		{
			Beep(1440, 30);
			playerhumanLocation.Y++;
		}
		if(keyPressed[K_A] && map[playerhumanLocation.X+1][playerhumanLocation.Y] != '2' || keyPressed[K_A] && map[playerhumanLocation.X][playerhumanLocation.Y+3] == '1')
		{
			Beep(1440, 30);
			playerhumanLocation.X--;
		}
		if(keyPressed[K_D] && map[playerhumanLocation.X+1][playerhumanLocation.Y+2] != '2' || keyPressed[K_D] && map[playerhumanLocation.X][playerhumanLocation.Y+3] == '1')
		{
			Beep(1440, 30);
			playerhumanLocation.X++;
		}
		//Spawning of enemies
		spawnenemy();
		Updatebarrel();//update location of barrels if it is active
		teleporters();// update location of teleporters
		extralifepowerup();
		firepowerup();
		freezepowerup();
		if(versus == true)
		{
			LoadMap(2);
			multiplayerdead();//gameover conditions for versus mode
		}
		else
		{
			LoadMap(1);
			monkeydead();//gameover conditions for single player
		}
	}
	//Pause function
	if(isKeyPressed(K_BACKSPACE))
	{
		pause = !pause;
	}

	// Return to the game menu if player hits the escape key
	if (keyPressed[K_ESCAPE])
	{
		gameStart();
	}
}

bool gameStart()
{
	init();
	gameStarted = false;
	//Menu Vars
	int *p = 0;
	int pointer = 0;
	p = &pointer;
	string Menu[3] = {"Start Angry Monkeys!", "2-Player Versus Mode", "Quit Angry Monkeys."};
	std::ifstream menuText;
	string menuBanner;

	//Navigating Menu
	while(true)	//Loop runs until user is not in menu screen
	{
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
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
		cout << "Please navigate through the menu and select your desired action using spacebar" << endl;
		for (int i = 0; i < 3; ++i)
		{
			if (i == *p)
			{
				//Selected options lights up to indicate selection
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
				Beep(840,30);
				cout << Menu[i] << " <-"<< endl;
			}
			else
			{
				//Unselected options are greyed
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
				cout << Menu[i] << endl;
			}
		}
		//Loop to detect user input and move accordingly until action is selected
		while(true)
		{
			if (isKeyPressed(VK_UP))
			{
				pointer -= 1;
				if (pointer == -1)
				{
					pointer = 2;
				}
				break;
			}
			else if (isKeyPressed(VK_DOWN))
			{
				pointer += 1;
				if (pointer == 3)
				{
					pointer = 0;
				}
				break;
			}
			//Execute action selected
			else if (isKeyPressed(VK_SPACE))
			{
				//Choice is made depending on what pointer points to
				switch (*p)
				{
				case STARTGAME:	//Starts the game
					{
						cout << "\n\nStarting the game now, please wait for a moment" << endl;
						Sleep(1500);
						versus = false;
						gameStarted = true;
						return gameStarted;
						break;
					} 

					//Explains the game
				case ABOUT:	
					{
						cout << "\n\nStarting 2-player versus game now, please wait for a moment" << endl;
						Sleep(1500);
						gameStarted = true;
						versus = true;
						return gameStarted;
						break;
						//menuText.open("Text/about.txt");
						////Opens up about text file
						//while(!menuText.eof())
						//{
						//	getline(menuText, menuBanner);
						//	cout << menuBanner << endl;

						//}
						//system("PAUSE");
						//menuText.close();
						//break;
					} 

				case QUITGAME:
					{
						cout << "\n\nQuitting the game now";
						Sleep(1000);
						g_quitGame = true;
						return gameStarted;
					} 
				}
				break;
			}
		}
		Sleep(100);
		cls();
	}
}

void render()// for drawing of objects only
{
	if (pause == false && Gameover.active == false)
	{
		// clear previous screen
		colour(0x0F);
		cls();

		//render the game
		read();
		// render time taken to calculate this frame
		gotoXY(70, 0);
		colour(0x1A);
		std::cout << 1.0 / deltaTime << "fps" << std::endl;

		gotoXY(0, 0);
		colour(0x59);
		std::cout << elapsedTime << "secs" << std::endl;

		// render character
		drawPlayer1();

		// render player human if in versus mode
		if(versus == true)
		{
			drawPlayer2();
		}
		// render barrel
		drawbarrel();

		//render teleporters
		if(elapsedTime>2)//time taken for teleporters to spawn
		{
			drawtele();
		}
		//draw banana
		drawbanana();

		//render life power up
		drawlife();

		//render fire power up
		drawfire();

		//render flames of fire power up
		drawflame();

		//render freeze powerup
		drawfreeze();

		//Draws the enemies
		//Spawns after 2 seconds
		drawenemy();
		
		//Draw dead enemy
		for(int i = 0; i<enemies; i++)
		{
			if(enemyList[i].health == 0)
			{
				drawdead(i);
			}
		}
	}
	if(Gameover.active == true && Gameover.type == 1)
	{
		drawNormalEnd();
	}
	if(Gameover.active == true && Gameover.type == 2)
	{
		drawplayer1Lose();
	}
	if(Gameover.active == true && Gameover.type == 3)
	{
		drawplayer2Lose();
	}
	else if (pause == true)
	{
		gotoXY(20,0);
		cout<<"Game is Paused, Press BACKSPACE to continue";
	}
}