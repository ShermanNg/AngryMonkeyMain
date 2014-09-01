// This is the main file for the game logic and function
//
//
#pragma once
#include "player.h"
#include "teleporters.h"
#include "Level.h"
#include "game.h"
#include "gameover.h"
#include "menu.h"


using std::cin;
using std::cout;
using std::endl;
using std::string;

double elapsedTime;
double deltaTime;
double dodgeTimer;
bool mapload = false;
bool keyPressed[K_COUNT];
bool pause;

extern bool gameStarted;
extern int modeSelected;

// Console size, width by height
COORD ConsoleSize = {80, 29};
COORD charLocation;
COORD playerhumanLocation;

void init()
{
	// Set precision for floating point output
	std::cout << std::fixed << std::setprecision(3);

	initConsole(ConsoleSize, "Angry Monkey");

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

	//initialise default single player map
	LoadMap();
	mapload = true;

	Highscoreload();
	elapsedTime = 0.0;
	pause = false;
}

void shutdown()
{
	// Reset to white text on black background
	colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    shutDownConsole();
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
	//Time elapses and update runs when unpaused and started
	if (pause == false && gameStarted == true && Gameover.active == false)
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
		if (keyPressed[K_RIGHT] && charLocation.X < ConsoleSize.X - 7)
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
		spawnenemy(dt);	//Spawns enemies
		Updatebarrel(dt);//update location of barrels if it is active
		teleporters();// update location of teleporters
		extralifepowerup();
		firepowerup();
		if(versus == true)
		{
			if(mapload == false)
			{
				LoadMap();
				mapload = true;
			}
			multiplayerdead();//gameover conditions for versus mode
		}
		else
		{
			if(mapload == false)
			{
				LoadMap();
				mapload = true;
			}
		
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
		mapload = false;
		gameStarted = false;
		init();
	}

	if(pause == false && gameStarted == false )
	{
		gameStart();
	}
}

void render()// for drawing of objects only
{
	//Draws menu for game
	if(gameStarted == false && pause == false)
	{
		drawMenu();
	}

	if (pause == false && Gameover.active == false && gameStarted == true)
	{
		// Clears the buffer with this colour attribute
        clearBuffer(0x1F);

		//render the game
		read();

		// render time taken to calculate this frame
		COORD dtime ={70, 0};
		double a = deltaTime;
		string framerate = static_cast<std::ostringstream*>(&(std::ostringstream()<<a))->str();
		writeToBuffer(dtime, framerate, 0x1A);

		COORD etime ={0, 0};
		string elap = static_cast<std::ostringstream*>(&(std::ostringstream()<<elapsedTime))->str();
		writeToBuffer(etime, elap, 0x59);
		
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
		if(elapsedTime>5)//time taken for teleporters to spawn
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

		//Draws the spawned enemies after 2 seconds
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
		//Pause message for menu
		if(gameStarted == false)
		{
			COORD a = {0,24};
			writeToBuffer(a,"Press BACKSPACE to Unpause");
		}
		//Pause message for in game
		else
		{
			COORD a = {20,0};
			writeToBuffer(a,"Game is Paused, Press BACKSPACE to continue");
		}
	}
	
	// Writes the buffer to the console, hence you will see what you have written
    flushBufferToConsole();
}