// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

double elapsedTime;
double deltaTime;
double randomTimer;

bool keyPressed[K_COUNT];
bool gameStarted = false;

COORD charLocation;
COORD consoleSize;
COORD teleporter1Location;
COORD teleporter2Location;
COORD lifepowerupLocation;

int barrelcount = 0;

enum States
{
	STARTGAME = 0,
	ABOUT,
	QUITGAME
};

struct Enemy{
	int health;
	int randNum;
	int dodgeChance;
	int respawnTime;
	bool toRight;
	bool canClimb;
	bool isClimbing;
	bool canMove;
	double moveTime;
	double climbTime;
	COORD position;
};

Enemy enemyList[6];

void initialiseEnemy(void)
{
	for(int i = 0; i < 6; i++)
	{
		//Spawns enemies at left side
		if(i < 3)
		{
		enemyList[i].health = 1;
		enemyList[i].randNum = rollDice();
		enemyList[i].dodgeChance = rollDice();
		enemyList[i].respawnTime = 3;
		enemyList[i].toRight = true;
		enemyList[i].canClimb = false;
		enemyList[i].isClimbing = false;
		enemyList[i].canMove = true;
		enemyList[i].climbTime = 0;
		enemyList[i].moveTime = 0;
		enemyList[0].position.X = 0;
		enemyList[1].position.X = 3;
		enemyList[2].position.X = 6;
		enemyList[i].position.Y = 25;
		}
		//Spawns enemies at right side
		if(i > 2)
		{
		enemyList[i].health = 1;
		enemyList[i].randNum = rollDice();
		enemyList[i].dodgeChance = rollDice();
		enemyList[i].respawnTime = 3;
		enemyList[i].toRight = true;
		enemyList[i].canClimb = false;
		enemyList[i].isClimbing = false;
		enemyList[i].canMove = true;
		enemyList[i].climbTime = 0;
		enemyList[i].moveTime = 0;
		enemyList[3].position.X = 64;
		enemyList[4].position.X = 67;
		enemyList[5].position.X = 70;
		enemyList[i].position.Y = 25;
		}
	}
}

struct Ladders{
	COORD position;
};

struct Barrel
{
	bool active;
	COORD position;
};
Barrel barrellist[3];
Barrel banana[3];
void intialisebarrel(void)//initialise barrel
{
	for(int i = 0; i<3; i++)
	{
		barrellist[i].position.X = 0;
		barrellist[i].position.Y= 0;
		barrellist[i].active=false;
	}
}

Ladders L_One, L_Two, L_Three, L_Four, L_Five, L_Six, L_Seven, L_Eight; 

void setLadders(Ladders& ladderPosition, COORD position)//Position of each ladder is defined
{
	//First Ladder
	L_One.position.X = 19;
	L_One.position.Y = 25;
	//Second Ladder
	L_Two.position.X = 39;
	L_Two.position.Y = 25;
	//Third Ladder
	L_Three.position.X = 59;
	L_Three.position.Y = 25;
	//Fourth Ladder
	L_Four.position.X = 30;
	L_Four.position.Y = 20;
	//Fifth Ladder
	L_Five.position.X = 50;
	L_Five.position.Y = 20;
	//Sixth Ladder
	L_Six.position.X = 19;
	L_Six.position.Y = 14;
	//Seventh Ladder
	L_Seven.position.X = 39;
	L_Seven.position.Y = 14;
	//Eigth Ladder
	L_Eight.position.X = 59;
	L_Eight.position.Y = 14;
}

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
    charLocation.Y = 2;

	// set the banana Coord and Status
	for(int i = 0; i<3; i++)
	{
		banana[i].active = true;
		banana[i].position.Y = 10;
	}
	//for testing purpose
	banana[0].active = false;
	banana[1].active = false;
	banana[2].active = false;
	banana[0].position.X = 20;
	banana[1].position.X = 40;
	banana[2].position.X = 60;

	//teleporter
	 teleporter1Location.X = (49);
	 teleporter1Location.Y = (16);
	 teleporter2Location.X = (20);
	 teleporter2Location.Y = (22);

	//location for life power up
	lifepowerupLocation.X = (1);
	lifepowerupLocation.Y = (1);

	// set the position of ladders
	setLadders(L_One,L_One.position);

	// set enemy variables
	initialiseEnemy();

    elapsedTime = 0.0;
}

//Rolling dice for chance to climb
int rollDice()
{
	srand(time(NULL));
	int randNum = 0;
	for(int i = 1; i < 6; i++)
	{
		randNum = rand() %6 +1;
	}
	return randNum;
}

struct timestamps{
	double timestamp;
	bool stamptime;
};

timestamps tele, life, fire;

void settimeStamps(timestamps& timing,double timestamp, bool stamptime)
{
	//teleporter
	tele.timestamp = 0.0;
	tele.stamptime = true;

	//life powerup
	life.timestamp = 0.0;
	life.stamptime = true;

	//fire powerup
	fire.timestamp = 0.0;
	life.stamptime = true;
}

struct powerups{
	bool present;
};

powerups plife, pfire;


void setpowerups(powerups& spawning,bool present)
{
	//life powerup
	plife.present = false;

	//fire powerup
	pfire.present = false;
}

void extralifepowerup()
{
	bool getpowerup = true;//able tp pick up power up

	//change random lifepowerup location
	if(plife.present == false)
	{
		lifepowerupLocation.X = (rand() % 72 + 3);
		lifepowerupLocation.Y = (4);
	}
	//lifepowerup random spawn time
	if(life.stamptime == true && plife.present == false)
	{
		life.timestamp = elapsedTime;
		life.stamptime = false;
	}
	if(elapsedTime >life.timestamp + rand() % 10 + 5 && plife.present == false)//change random spawn timing here
	{
		plife.present = true;
		getpowerup = true;
		life.stamptime = true;
	}

	//lifepowerup
	if(getpowerup == true)
	{
		if(charLocation.X == lifepowerupLocation.X)
		{
			for(int i = 0; i<3; i++)
			{
				if(banana[i].active == false)
				{
					banana[i].active = true;
				}
			}
		}
		plife.present = false;
		getpowerup = false;
	}
}
void teleporters()
{
	bool telecd = false;// teleporter cooldown

	if(tele.stamptime == true)
	{
		tele.timestamp = elapsedTime;
		tele.stamptime = false;
	}

	//change in teleporter location timing
	if(elapsedTime > tele.timestamp + 5)
	{
		//location for teleporter1
		teleporter1Location.Y = (16);
		teleporter1Location.X = (rand() % 76 + 1);

		//location for teleporter2
		teleporter2Location.X = (rand() % 76 + 1);
		if(rand() % 2 + 1 == 2)
		{
			teleporter2Location.Y = (22);
		}
		else
		{
			teleporter2Location.Y = (28);
		}
		tele.stamptime = true;
	}

	//teleport enemies
	if(elapsedTime>rand() % 1 + 2)//time before teleporters spawn
	{
		//teleporter 1
		for(int i = 0; i<6; i++)
		{
		if(enemyList[i].position.Y == teleporter1Location.Y - 2 && enemyList[0].position.X == teleporter1Location.X)
		{
			if(telecd == false)
			{
				enemyList[i].position.Y = teleporter2Location.Y-2;
				enemyList[i].position.X = teleporter2Location.X;
				telecd = true;
			}
		}
		}
		//teleporter 2
		for(int i = 0; i<6; i++)
		{
		if(enemyList[i].position.Y == teleporter2Location.Y - 2&& enemyList[i].position.X == teleporter2Location.X)
		{
			if(telecd == false)
			{
				enemyList[i].position.Y = teleporter1Location.Y-2;
				enemyList[i].position.X = teleporter1Location.X;
				telecd = true;
			}
		}
	}
	telecd = false;
	}
}

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
	keyPressed[K_RETURN] = isKeyPressed(VK_RETURN);
}

void update(double dt)
{
    // get the delta time
    elapsedTime += dt;
    deltaTime = dt;
	randomTimer += dt;
	int printL = 0;
	int printR = 0;
	int leftNum = 0;
	int rightNum = 0;
	char player[3][3] = {
		{' ','O',' '},
		{'-','|','-'},
		{'/',' ','\\'}
	};

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
		if(barrelcount<3)
		{
		barrelshooting(charLocation);
		barrelcount++;
		}
		if(barrelcount == 3)
		{
			barrelcount = 0;
		}
	}

	teleporters();
	extralifepowerup();
	

    // quits the game if player hits the escape key
    if (keyPressed[K_ESCAPE])
      {
		  gameStart();
	  }
}

void DrawMap2 (void)
{

	int y =  consoleSize.Y;

	gotoXY(0,y-24);
	colour(0x3C);
	std::cout << "                                                                                ";

	gotoXY(0,y-0);
	colour(0x3C);
	std::cout << "                                                                                ";

	gotoXY(0,y-6);
	colour(0x3C);
	std::cout << "                                                                                ";

	gotoXY(0,y-12);
	colour(0x3C);
	std::cout << "                                                                                ";

	gotoXY(0,y-18);
	colour(0x3C);
	std::cout << "                                                                                ";


	for (int j = 0; j < 6; ++j)
	{
		gotoXY(20,y-j);
		colour(0x4D);
		std::cout << "  "<<endl;

		gotoXY(40,y-j);
		colour(0x4D);
		std::cout << "  "<<endl;

		gotoXY(60,y-j);
		colour(0x4D);
		std::cout << "  "<<endl;


	}


	for (int j =6; j < 12; ++j)
	{
		gotoXY(30,y-j);
		colour(0x4D);
		std::cout << "  "<<endl;

		gotoXY(50,y-j);
		colour(0x4D);
		std::cout << "  "<<endl;
	}

	for (int j = 12; j < 18; ++j)
	{
		gotoXY(20,y-j);
		colour(0x4D);
		std::cout << "  "<<endl;

		gotoXY(40,y-j);
		colour(0x4D);
		std::cout << "  "<<endl;

		gotoXY(60,y-j);
		colour(0x4D);
		std::cout << "  "<<endl;
	}

}

void DrawMap1 (void)
{

	int y =  consoleSize.Y;

	gotoXY(0,y-0);
	colour(0x3C);
	std::cout << "                                                                                ";

	gotoXY(0,y-6);
	colour(0x3C);
	std::cout << "                                                                                ";

	gotoXY(0,y-12);
	colour(0x3C);
	std::cout << "                                                                                ";

	gotoXY(0,y-18);
	colour(0x3C);
	std::cout << "                                                                                ";


	for (int j = 6; j < 12; ++j)
	{
		gotoXY(20,y-j);
		colour(0x4D);
		std::cout << "  "<<endl;

		gotoXY(40,y-j);
		colour(0x4D);
		std::cout << "  "<<endl;

		gotoXY(60,y-j);
		colour(0x4D);
		std::cout << "  "<<endl;


	}


	for (int j =0; j < 6; ++j)
	{
		gotoXY(30,y-j);
		colour(0x4D);
		std::cout << "  "<<endl;

		gotoXY(50,y-j);
		colour(0x4D);
		std::cout << "  "<<endl;
	}


	for (int j =12; j < 18; ++j)
	{
		gotoXY(30,y-j);
		colour(0x4D);
		std::cout << "  "<<endl;

		gotoXY(50,y-j);
		colour(0x4D);
		std::cout << "  "<<endl;
	}

}

bool gameStart()
{

	//Menu Vars
	int *p = 0;
	int pointer = 0;
	p = &pointer;
	string Menu[3] = {"Start Angry Monkeys!", "About Angry Monkeys", "Quit Angry Monkeys."};
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
		cout << "Please navigate through the menu and select your desired action" << endl;
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
			else if (isKeyPressed(VK_RETURN))
			{
				//Choice is made depending on what pointer points to
				switch (*p)
				{
					case STARTGAME:	//Starts the game
					{
						cout << "\n\nStarting the game now, please wait for a moment" << endl;
						Sleep(1500);
						gameStarted = true;
						return gameStarted;
						break;
					} 

					//Explains the game
					case ABOUT:	
					{
						menuText.open("Text/about.txt");
						//Opens up about text file
						while(!menuText.eof())
						{
							getline(menuText, menuBanner);
							cout << menuBanner << endl;
							
						}
						system("PAUSE");
						menuText.close();
						break;
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

void render()
{
	int print = 0;
	char player[3][3] = {
		{' ','O',' '},
		{'-','|','-'},
		{'/',' ','\\'}
	};
    // clear previous screen
    colour(0x0F);
    cls();

    //render the game

	DrawMap2();

    // render time taken to calculate this frame
    gotoXY(70, 0);
    colour(0x1A);
    std::cout << 1.0 / deltaTime << "fps" << std::endl;
  
    gotoXY(0, 0);
    colour(0x59);
    std::cout << elapsedTime << "secs" << std::endl;

    // render character
    gotoXY(charLocation);
    colour(0x0C);
    for(int i = 0; i<=2; ++i)
	{
		for(int j = 0; j<=2; ++j)
		{
			std::cout << player[i][j];
		}
		print++;
		gotoXY(charLocation.X,charLocation.Y+print);
	}

	Updatebarrel();
	spawnEnemy();

	//render teleporters
	if(elapsedTime>2)//time taken for teleporters to spawn
	{
		gotoXY(teleporter1Location);
		colour(0x0C);
		std::cout << (char)5;
		std::cout << (char)5;
		gotoXY(teleporter2Location);
		colour(0x0C);
		std::cout << (char)5;
		std::cout << (char)5;
	}
    //draw banana
	for(int i = 0; i<3; i++)
	{
		if(banana[i].active == true)
		{
			gotoXY(banana[i].position);
			colour(0x0E);
			std::cout<<"@@";
		}
	}

	//render life power up
	if(plife.present == true)
	{
		gotoXY(lifepowerupLocation);
		colour(0x0C);
		std::cout<<(char)3;
	}
	if(isKeyPressed(VK_F1))
	{
		pause();
	}
}

void pause()
{
	system("PAUSE");
} 

void spawnEnemy()
{
	//Enemy and movement code
	int printLine = 0;
	char enemy[3][3] = {
		{' ','O',' '},
		{'-','|','-'},
		{'/',' ','\\'}
	};
		
	//Spawns 6 enemies
	for(int number = 0; number < 6; number++)
	{
		//Spawns enemies after 4 seconds
		if (elapsedTime>4 && enemyList[number].position.X >= 0)
		{
			//Designated spawn point
			gotoXY(enemyList[number].position.X,enemyList[number].position.Y);

			//Colors enemy
			colour(0x0c);
			
			//Drawing of enemy
			for(int j = 0; j<=2; ++j)
			{
				for(int k = 0; k<=2; ++k)
				{
					cout << enemy[j][k];
				}
				printLine++;
				gotoXY(enemyList[number].position.X,enemyList[number].position.Y+printLine);
			}
			//Moves enemy every second
			moveEnemy();

			//Resetting Y Pos after draw
			printLine=0;
		}
	}

}

void moveEnemy()
{
	//AI 1.0 Movement
	//Moves the 6 enemies
	for(int number = 0; number < 6; number++)
	{
		//Only move if alive
		if(enemyList[number].health == 1)
		{
			//If allowed to move after climbing conditions
			if(enemyList[number].canMove == true)
			{
				//Slowing down movement due to loop
				enemyList[number].moveTime += 0.1;
				if(enemyList[number].moveTime > 1)
				{
					
					

					//Allows moving to the LEFT side upon reaching the corner
					if(enemyList[number].position.X > 76)
					{
						enemyList[number].toRight = false;
					}
					//Moving to opposite side(Left)
					if(enemyList[number].toRight == false)
					{
						enemyList[number].position.X--;

						//AI 1.1 Evasive movement(Dodge right)
						for(int i = 0; i < 3; i++)
						{
							//Checks for: on same position, above and active barrels
							if(enemyList[number].position.X == barrellist[i].position.X && barrellist[i].active == true && barrellist[i].position.Y < enemyList[number].position.Y)
							{
								//Checks the random number for to dodge(50%, chance reset every frame)
								if(enemyList[number].dodgeChance < 4)
								{
									enemyList[number].position.X+=3;
									enemyList[number].dodgeChance = rollDice();
								}
							}
						}
						////After climbing
						//if(enemyList[number].position.Y == false)
						//{
						//	//Random movement
						//	if(enemyList[number].randNum < 4)
						//	{
						//		enemyList[number].position.X--;
						//	}
						//	else if(enemyList[number].randNum > 3)
						//	{
						//		enemyList[number].position.X++;
						//	}
						//}
						}
					
					
					//Allows moving to the RIGHT side upon reaching the corner
					if(enemyList[number].position.X < 1)
					{
						enemyList[number].toRight = true;
					}
					//Moving right side after reaching end of left side
					if(enemyList[number].toRight == true)
					{
						enemyList[number].position.X++;

						//AI 1.1 Evasive movement(Dodge left)
						for(int i = 0; i < 3; i++)
						{
							//Checks for: on same position, above and active barrels
							if(enemyList[number].position.X == barrellist[i].position.X && barrellist[i].active == true && barrellist[i].position.Y < enemyList[number].position.Y)
							{
								//Checks the random number for to dodge(50%, chance reset every frame)
								if(enemyList[number].dodgeChance < 4)
								{
									enemyList[number].position.X-=3;
									enemyList[number].dodgeChance = rollDice();
								}
							}
						}
					}

					//Reset movement time for delay
					enemyList[number].moveTime = 0;
				}
			}
			//AI 1.2 Climbing
			enemyClimb();
		}

		//AI 1.3 Dodge chance reset every 3 seconds
		if(randomTimer > 0.1)
		{
			randomTimer = 0;
			enemyList[number].dodgeChance = rollDice();
		}
	}
	//else
	//{
	//	death(); // CALL death function here DO NOT ADD HERE!!
	//}
}

void enemyClimb()
{
	//AI 1.2 Climbing
	//Climbing ladders only if collided at a 50% rate
	for(int number = 0; number < 6; number++)
	{
		if(enemyList[number].randNum < 7)
		{
			enemyList[number].canClimb = (climbCheck(enemyList[number].position.X, enemyList[number].position.Y, enemyList[number].isClimbing));
			//If enabled to climb
			if(enemyList[number].canClimb == true)
			{
				//Current state is climbing
				enemyList[number].isClimbing = true;

				//Checks if enemy is still climbing
				if(enemyList[number].isClimbing == true)
				{
					enemyList[number].climbTime += 0.05;
					if(enemyList[number].climbTime > 0.2)
					{
						enemyList[number].position.Y--;
						enemyList[number].canMove = false;
						enemyList[number].climbTime = 0;
					}
				}
				//Alligns to platform after climbing
				climbAlign(number);
			}
			//Reroll chance
			else
			{
				enemyList[number].randNum = rollDice();
			}
		}
	}
}

void climbAlign(int identity)
{
	//Adjusts enemy to platform's Y COORD after climbing(Lowest lvl)
	if(enemyList[identity].position.Y == 20)
	{
		enemyList[identity].canMove = true;
		enemyList[identity].canClimb = false;
		enemyList[identity].isClimbing = false;
		//Randomize numbers again
		if(enemyList[identity].isClimbing == false)
		{
			enemyList[identity].randNum = rollDice();

			if(enemyList[identity].randNum < 4)
			{
				enemyList[identity].toRight = false;
			}
			else if(enemyList[identity].randNum > 3)
			{
				enemyList[identity].toRight = true;
			}

		}
	}
	//Adjusts enemy to platform Y COORD after climbing(mid lvl)
	else if(enemyList[identity].position.Y == 14)
	{
		enemyList[identity].canMove = true;
		enemyList[identity].canClimb = false;
		enemyList[identity].isClimbing = false;
		//Randomize numbers again
		if(enemyList[identity].isClimbing == false)
		{
			enemyList[identity].randNum = rollDice();

			if(enemyList[identity].randNum < 4)
			{
				enemyList[identity].toRight = false;
			}
			else if(enemyList[identity].randNum > 3)
			{
				enemyList[identity].toRight = true;
			}

		}
	}
	//Adjusts enemy to platform Y COORD after climbing(highest lvl)
	else if(enemyList[identity].position.Y == 8)
	{
		enemyList[identity].canMove = true;
		enemyList[identity].canClimb = false;
		enemyList[identity].isClimbing = false;
		//Randomize numbers again
		if(enemyList[identity].isClimbing == false)
		{
			enemyList[identity].randNum = rollDice();
			if(enemyList[identity].randNum < 4)
			{
				enemyList[identity].toRight = false;
			}
			else if(enemyList[identity].randNum > 3)
			{
				enemyList[identity].toRight = true;
			}

		}
	}
}

bool climbCheck(int posX, int posY, bool isClimbing)
{
	int y = consoleSize.Y;
	//AI 1.3 Evasive climbing
	for(int i = 0; i < 3; i++)
	{
		//Climbs only when no crates are falling.
		if(posX == barrellist[i].position.X && barrellist[i].active == true && barrellist[i].position.Y < posY)
		{
			//If is climbing, cannot change to unable to climb
			if(!(isClimbing))
			{
				return false;
			}
		}
	}
			
	//Loop for ladder length for first level
	for(int i = 0;i < 8; ++i)
	{
		y--;
		//Checking collision against first ladder
		if(posX == L_One.position.X && posY == y)
		{
			return true;
		}
		//Checking collision against second ladder
		else if(posX == L_Two.position.X && posY == y)
		{
			return true;
		}
		//Checking collision against third ladder
		else if(posX == L_Three.position.X && posY == y)
		{
			return true;
		}
					
	}

	//Loop for ladder length for second level
	for(int i =0;i < 6; ++i)
	{
		y--;
		//Checking collision against first ladder
		if(posX == L_Four.position.X && posY == y)
		{
			return true;
		}
		//Checking collision against second ladder
		if(posX == L_Five.position.X && posY == y)
		{
			return true;
		}
	}

	//Loop for ladder length for third level
	for(int i = 0;i < 6; ++i)
	{
		y--;
		//Checking collision against first ladder
		if(posX == L_Six.position.X && posY == y)
		{
			return true;
		}
		//Checking collision against second ladder
		if(posX == L_Seven.position.X && posY == y)
		{
			return true;
		}
		//Checking collision against third ladder
		if(posX == L_Eight.position.X && posY == y)
		{
			return true;
		}
	}
		return false;
}

void barrelshooting(COORD unit)
{
	for(int i = 0; i<3; i++)
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
	for(int i = 0; i<3; i++)
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
	for(int i = 0; i<3; i++)
	{
		if (barrellist[i].active == true)
		{
			if(barrellist[i+1].active == false)// error usage
			{
			drawbarrel();
			}
			if(barrellist[i].position.Y >= 27)
			{
				barrellist[i].position.Y = 28;
			}
			else
			{
				barrellist[i].position.Y+=2;
			}
			if(barrellist[i].position.Y == consoleSize.Y-1)
			{
				barrellist[i].position.X = charLocation.X;
				barrellist[i].position.Y = charLocation.Y+1;
				barrellist[i].active = false;
			}
		}
	}
}