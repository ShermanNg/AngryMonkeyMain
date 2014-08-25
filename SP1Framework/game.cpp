// This is the main file for the game logic and function
//
//
#define barrelNum 3 // num of barrel allowed at one time on screen
#define enemies 8 //Size of enemies
#define sizeX 8 //Size of X Coord Array
#define sizeY 8 //Size of Y Coord Array
#define sizeLadders 8 //Size of Ladders Array

#include "game.h"

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
int highscore;
int Score;

COORD charLocation;
COORD consoleSize;
COORD teleporter1Location;
COORD teleporter2Location;
COORD lifepowerupLocation;

// Enum of Game State
enum States
{
	STARTGAME = 0,
	ABOUT,
	QUITGAME
};

//Struct of enemy properties
struct Enemy
{
	int health;
	int randNum;
	int dodgeChance;
	bool toRight;
	bool canClimb;
	bool isClimbing;
	bool canMove;
	bool isAlive;
	bool isSpecial;
	double respawnTimer;
	COORD position;
};

//Struct of ladder properties
struct Ladders{
	COORD position;
};

Ladders ladderList[sizeLadders];

int ladderX[sizeLadders] = {19, 39, 59, 30, 50, 19, 39, 59};
int ladderY[sizeLadders] = {25, 25, 25, 20, 20, 14, 14, 14};

//Using defined array size
Enemy enemyList[enemies];	//Enemy count
bool enemyType[enemies] = {true, true, false, false, false, false, false, false};
int enemyX[sizeX] = {25, 30, 0, 3, 6, 64, 67, 70};//left enemy X=COORD
int enemyY[sizeY] = {26, 26, 26, 26, 26, 26, 26, 26};//Standard enemy Y COORD

// Initialise enemies
void initialiseEnemy(void)
{
	for(int i = 0; i < 8; i++)
	{	
		//initialising stats
		enemyList[i].health = 1;
		enemyList[i].randNum = rollDice();
		enemyList[i].dodgeChance = rollDice();
		enemyList[i].toRight = true;
		enemyList[i].canClimb = false;
		enemyList[i].isClimbing = false;
		enemyList[i].canMove = true;
		enemyList[i].isAlive = true;
		enemyList[i].respawnTimer = 3;
		enemyList[i].position.X = enemyX[i];
		enemyList[i].position.Y = enemyY[i];
		enemyList[i].isSpecial = enemyType[i];
	}
}

//Position of each ladder is defined
void initialiseLadders(void)
{
	for(int i = 0; i < 8; i++)
	{
		ladderList[i].position.X = ladderX[i];
		ladderList[i].position.Y = ladderY[i];
	}
}

//Struct of Barrel properties
struct Barrel
{
	bool active;
	COORD position;
};

Barrel barrellist[3];
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

void init()
{

	LoadMap();
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
	banana[0].position.X = 20;
	banana[1].position.X = 40;
	banana[2].position.X = 60;

	//teleporter
	teleporter1Location.X = (49);
	teleporter1Location.Y = (16);
	teleporter2Location.X = (20);
	teleporter2Location.Y = (22);

	//initialise location for life power up
	lifepowerupLocation.X = (0);
	lifepowerupLocation.Y = (0);

	// set the position of ladders
	initialiseLadders();

	// set enemy variables
	initialiseEnemy();

	Highscoreload();
	elapsedTime = 0.0;
	pause = false;
}

//Rolling dice for chance to climb
int rollDice()
{
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
	bool getpowerup = true;//able to pick up power up
	int randomloop = rand() % 24 + 1;

	//change random lifepowerup location
	if(plife.present == false)
	{
		for(int n = 0;n < randomloop;n++)
		{
			if(randomloop - 1 == n)
			{
				lifepowerupLocation.X = (n * 3) + 1;
			}
		}
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
	if(getpowerup == true && plife.present == true)
	{
		if(charLocation.X == lifepowerupLocation.X)
		{
			for(int i = 0; i<3; i++)
			{
				if(banana[i].active == false)
				{
					banana[i].active = true;
					break;
				}
			}
			plife.present = false;
			getpowerup = false;
		}
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
			teleporter2Location.Y = (27);
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
			if(enemyList[i].position.Y == teleporter2Location.Y - 2 && enemyList[i].position.X == teleporter2Location.X)
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
	keyPressed[K_F1] = isKeyPressed(VK_F1);
}

void update(double dt)
{
	if (pause == false)
	{
		Score = elapsedTime*1;
		// get the delta time
		elapsedTime += dt;
		deltaTime = dt;
		dodgeTimer += dt;
		Score = elapsedTime*1;
		int printL = 0;
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
		//Spawning of enemies
		for(int i = 0; i < enemies; i++)
		{
			if(enemyList[i].isAlive == true)
			{
				moveEnemy(i);
			}
			//If enemy is not alive
			else if(enemyAlive(i) == false)
			{
				//Respawn timer
				enemyList[i].respawnTimer -= dt;
				//If countdown to 0 and below, respawns
				if(enemyList[i].respawnTimer < 0)
				{
					//Restores the selected enemy and properties to alive status
					enemyList[i].health = 1;
					enemyList[i].canMove = true;
					enemyList[i].isAlive = true;
					enemyList[i].respawnTimer = 3;
					enemyList[i].position.X = enemyX[i];
					enemyList[i].position.Y = enemyY[i];
				}
			}
		}

		Updatebarrel();//update location of barrels if it is active
		teleporters();// update location of teleporters
		extralifepowerup();
		monkeydead();// update banana status
	}
	//Pause function
	if(isKeyPressed(VK_F1))
	{
		pause = !pause;
	}

	// quits the game if player hits the escape key
	if (keyPressed[K_ESCAPE])
	{
		gameStart();
	}
}

void DrawMap2 (void)
{
	//int y =  consoleSize.Y;

	//gotoXY(0,y-24);
	//colour(0x3C);
	//std::cout << "                                                                                ";

	//gotoXY(0,y-1);
	//colour(0x3C);
	//std::cout << "                                                                                ";

	//gotoXY(0,y-6);
	//colour(0x3C);
	//std::cout << "                                                                                ";

	//gotoXY(0,y-12);
	//colour(0x3C);
	//std::cout << "                                                                                ";

	//gotoXY(0,y-18);
	//colour(0x3C);
	//std::cout << "                                                                                ";


	//for (int j = 0; j < 6; ++j)
	//{
	//	gotoXY(20,y-j);
	//	colour(0x4D);
	//	std::cout << "  "<<endl;

	//	gotoXY(40,y-j);
	//	colour(0x4D);
	//	std::cout << "  "<<endl;

	//	gotoXY(60,y-j);
	//	colour(0x4D);
	//	std::cout << "  "<<endl;


	//}


	//for (int j =6; j < 12; ++j)
	//{
	//	gotoXY(30,y-j);
	//	colour(0x4D);
	//	std::cout << "  "<<endl;

	//	gotoXY(50,y-j);
	//	colour(0x4D);
	//	std::cout << "  "<<endl;
	//}

	//for (int j = 12; j < 18; ++j)
	//{
	//	gotoXY(20,y-j);
	//	colour(0x4D);
	//	std::cout << "  "<<endl;

	//	gotoXY(40,y-j);
	//	colour(0x4D);
	//	std::cout << "  "<<endl;

	//	gotoXY(60,y-j);
	//	colour(0x4D);
	//	std::cout << "  "<<endl;
	//}
	read();
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
	init();
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

void render(int a)// for drawing of objects only
{
	if (pause ==false)
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
		if(a == 1)
		{
			DrawMap2();
		}

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

		// render barrel
		drawbarrel();

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
			if(banana[i].active == false)
			{
				gotoXY(banana[i].position);
				colour(0x0E);
				std::cout<<"gone";
			}
		}

		//render life power up
		if(plife.present == true)
		{
			gotoXY(lifepowerupLocation);
			colour(0x0C);
			std::cout<<(char)3;
		}

		//Draws the enemies
		int printLine = 0;
		char enemy[3][3] = {
			{' ','O',' '},
			{'-','|','-'},
			{'/',' ','\\'}
		};

		//Spawns after 2 seconds
		if(elapsedTime > 0)
		{
			for(int i = 0; i < enemies; i++)
			{
				//Spawns enemies after 4 seconds
				if (enemyList[i].position.X >= 0 && enemyList[i].isAlive == true)
				{
					//Designated spawn point
					gotoXY(enemyList[i].position.X,enemyList[i].position.Y);

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
						gotoXY(enemyList[i].position.X,enemyList[i].position.Y+printLine);
					}
					//Resetting Y Pos after draw
					printLine=0;
				}
			}
		}
	}

	else if (pause == true)
	{
		gotoXY(40,0);
		cout<<"Paused";
	}

}

void moveEnemy(int identity)
{
	//AI 1.0 Movement

	//Damage enemies
	for(int i = 0; i < 3; i++)
	{
		//Collision against enemy bodies
		if(enemyList[identity].position.X == barrellist[i].position.X && enemyList[identity].position.Y == barrellist[i].position.Y
			|| enemyList[identity].position.X == barrellist[i].position.X && enemyList[identity].position.Y+1 == barrellist[i].position.Y
			|| enemyList[identity].position.X == barrellist[i].position.X && enemyList[identity].position.Y+2 == barrellist[i].position.Y
			|| enemyList[identity].position.X+1 == barrellist[i].position.X && enemyList[identity].position.Y == barrellist[i].position.Y
			|| enemyList[identity].position.X+1 == barrellist[i].position.X && enemyList[identity].position.Y+1 == barrellist[i].position.Y
			|| enemyList[identity].position.X+1 == barrellist[i].position.X && enemyList[identity].position.Y+2 == barrellist[i].position.Y
			|| enemyList[identity].position.X-1 == barrellist[i].position.X && enemyList[identity].position.Y == barrellist[i].position.Y
			|| enemyList[identity].position.X-1 == barrellist[i].position.X && enemyList[identity].position.Y+1 == barrellist[i].position.Y
			|| enemyList[identity].position.X-1 == barrellist[i].position.X && enemyList[identity].position.Y+2 == barrellist[i].position.Y)
		{
			enemyList[identity].health = 0;
			break;
		}
	}
	//Only move if alive
	if(enemyList[identity].health == 1)
	{
		//If allowed to move after climbing conditions
		if(enemyList[identity].canMove == true)
		{



			//Allows moving to the LEFT side upon reaching the corner
			if(enemyList[identity].position.X > 76)
			{
				enemyList[identity].toRight = false;
			}
			//Moving to opposite side(Left)
			if(enemyList[identity].toRight == false)
			{
				enemyList[identity].position.X--;

				//AI 1.1 Evasive movement(Dodge right)
				for(int i = 0; i < 3; i++)
				{
					//Checks for: on same position, above and active barrels and not climbing
					if(enemyList[identity].position.X == barrellist[i].position.X && barrellist[i].active == true && barrellist[i].position.Y < enemyList[identity].position.Y && enemyList[identity].isClimbing == false)
					{
						//Checks the random number for to dodge(50%, chance reset every frame)
						if(enemyList[identity].dodgeChance < 4)
						{
							enemyList[identity].position.X+=4;
							enemyList[identity].dodgeChance = rollDice();
						}
					}
				}
			}


			//Allows moving to the RIGHT side upon reaching the corner
			if(enemyList[identity].position.X < 1)
			{
				enemyList[identity].toRight = true;
			}
			//Moving right side after reaching end of left side
			if(enemyList[identity].toRight == true)
			{
				enemyList[identity].position.X++;

				//AI 1.1 Evasive movement(Dodge left)
				for(int i = 0; i < 3; i++)
				{
					//Checks for: on same position, above and active barrels and not climbing
					if(enemyList[identity].position.X == barrellist[i].position.X && barrellist[i].active == true && barrellist[i].position.Y < enemyList[identity].position.Y && enemyList[identity].isClimbing == false)
					{
						//Checks the random number for to dodge(50%, chance reset every frame)
						if(enemyList[identity].dodgeChance < 4)
						{
							enemyList[identity].position.X-=4;
							enemyList[identity].dodgeChance = rollDice();
						}
					}
				}
			}
		}

		//AI 1.2 Climbing
		enemyClimb(identity);

	}
	else if(enemyList[identity].health == 0)
	{
		enemyAlive(identity); 
	}

	//AI 1.3 Dodge chance reset every 1.5 seconds
	if(dodgeTimer > 1.5)
	{
		dodgeTimer = 0;
		enemyList[identity].dodgeChance = rollDice();
	}
}

void enemyClimb(int identity)
{
	//AI 1.2 Climbing
	//Climbing ladders only if collided at a 50% rate
	if(enemyList[identity].randNum < 7)
	{
		enemyList[identity].canClimb = (climbCheck(identity, enemyList[identity].isClimbing));

		//If enabled to climb
		if(enemyList[identity].canClimb == true)
		{
			//Climb only if alive
			if(enemyList[identity].health == 1)
			{
				//Starts climbing
				enemyList[identity].position.Y--;
				enemyList[identity].isClimbing = true;
				enemyList[identity].canMove = false;

				//Alligns to platform after climbing
				climbAlign(identity);
			}
			//Enemy is no longer alive
			else if(enemyList[identity].health == 0)
			{
				enemyAlive(identity); 
			}
		}

	}
	//Reroll chance
	else
	{
		enemyList[identity].randNum = rollDice();
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

bool climbCheck(int identity, bool isClimbing)
{
	int y = consoleSize.Y;

	//AI 1.3 Evasive climbing
	for(int i = 0; i < 3; i++)
	{
		//Climbs only when no crates are falling.
		if(enemyList[identity].position.X == barrellist[i].position.X && barrellist[i].active == true && barrellist[i].position.Y < enemyList[identity].position.Y)
		{
			//Special enemy can jump off and avoid dropping barrels while climbing
			if(isClimbing && enemyList[identity].isSpecial == true)
			{
				//enemyList[identity].position.X++;
				//enemyList[identity].position.Y++;
				//return false;
			}

		}
		////If is climbing, cannot change to unable to climb
		//else if(enemyList[identity].isClimbing == true)
		//{
		//	return true;
		//}
	}

	//Loop for ladder length for first level
	for(int i = 0;i < 8; ++i)
	{
		y--;
		//Checking collision against first ladder
		if(enemyList[identity].position.X == ladderList[0].position.X && enemyList[identity].position.Y == y)
		{
			return true;
		}
		//Checking collision against second ladder
		else if(enemyList[identity].position.X == ladderList[1].position.X && enemyList[identity].position.Y == y)
		{
			return true;
		}
		//Checking collision against third ladder
		else if(enemyList[identity].position.X == ladderList[2].position.X && enemyList[identity].position.Y == y)
		{
			return true;
		}

	}

	//Loop for ladder length for second level
	for(int i =0;i < 6; ++i)
	{
		y--;
		//Checking collision against first ladder
		if(enemyList[identity].position.X == ladderList[3].position.X && enemyList[identity].position.Y == y)
		{
			return true;
		}
		//Checking collision against second ladder
		else if(enemyList[identity].position.X == ladderList[4].position.X && enemyList[identity].position.Y == y)
		{
			return true;
		}
	}

	//Loop for ladder length for third level
	for(int i = 0;i < 6; ++i)
	{
		y--;
		//Checking collision against first ladder
		if(enemyList[identity].position.X == ladderList[5].position.X && enemyList[identity].position.Y == y)
		{
			return true;
		}
		//Checking collision against second ladder
		else if(enemyList[identity].position.X == ladderList[6].position.X && enemyList[identity].position.Y == y)
		{
			return true;
		}
		//Checking collision against third ladder
		else if(enemyList[identity].position.X == ladderList[7].position.X && enemyList[identity].position.Y == y)
		{
			return true;
		}
	}
	//Unable to climb at not at a ladder
	return false;
}

bool enemyAlive(int identity)
{
	//Vertical Line for printing enemes
	int printLine = 0;

	//Disables movement
	enemyList[identity].canMove = false;

	//Go to specific enemy coord
	gotoXY(enemyList[identity].position.X,enemyList[identity].position.Y);

	//Clears up dead enemy
	for(int j = 0; j<=2; ++j)
	{
		for(int k = 0; k<=2; ++k)
		{
			cout << " ";
		}
		printLine++;
		gotoXY(enemyList[identity].position.X,enemyList[identity].position.Y+printLine);
	}

	//Enemy is no longer alive
	enemyList[identity].isAlive = false;

	//Returns is not alive status
	return enemyList[identity].isAlive;
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

void monkeydead()
{
	int death = 0;
	for(int i = 0; i<enemies; i++)
	{
		COORD temp = enemyList[i].position;
		for(int i = 0; i<3; i++)
		{
			if(temp.X == banana[i].position.X && temp.Y+2 == banana[i].position.Y && banana[i].active == true)
			{
				banana[i].active = false;
			}
		}
	}
	for(int i = 0; i<3; i++)
	{
		if(banana[i].active == false)
		{
			death++;
		}
	}
	if(death==3)
	{
		death = 0;//reset temp death value
		showgameover();
		HighscoreCheck();
	}

}

void HighscoreCheck()
{
	int i = highscore;
	if (i < Score)
	{
		HighscoreSave();
	}

}

void showgameover()
{
	cls();
	gotoXY(13,consoleSize.Y/4);
	colour(0x0E);
	cout<<"*******************************************************";
	for(int i = 8; i<14; i++)
	{
		gotoXY(13,i);
		colour(0x0E);
		cout<<"*                                                     *";
	}
	gotoXY(13,consoleSize.Y/2);
	colour(0x0E);
	cout<<"* GameOver Press something to return to the main menu *";
	for(int i = 15; i<21; i++)
	{
		gotoXY(13,i);
		colour(0x0E);
		cout<<"*                                                     *";
	}
	gotoXY(13,21);
	colour(0x0E);
	cout<<"*******************************************************";
	getchar();
	if(getchar())
	{
		cls();
		gameStart();
	}
	//for(int i = 0; i<3; i++)//reset banana status
	//{
	//	banana[i].active = true;
	//}
	//gameStart();
}

void HighscoreSave (void)
{
	std::ofstream myfile;
	myfile.open ("HighScore.txt", std::ios::in|std::ios::trunc);
	myfile << Score <<endl;
	myfile.close();
}

int Highscoreload(void)
{
	string line;
	std::ifstream myfile ("HighScore.txt");
	if (myfile.is_open())
	{
		getline (myfile,line);
		std::stringstream convert(line);
		if ( !(convert >> highscore) )//give the value to Result using the characters in the string
			return highscore;

		myfile.close();
	}
}