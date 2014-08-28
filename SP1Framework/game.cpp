// This is the main file for the game logic and function
//
//
#define barrelNum 3 // num of barrel allowed at one time on screen
#define enemies 8 //Size of enemies
#define sizeX 8 //Size of X Coord Array
#define sizeY 8 //Size of Y Coord Array
#define sizeLadders 8 //Size of Ladders Array
#define endScreenNum 3// No of end screen in game
#define speed 0.3//General speed

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

COORD charLocation;
COORD playerhumanLocation;
COORD consoleSize;

// Enum of Game State
enum States
{
	STARTGAME = 0,
	ABOUT,
	QUITGAME
};


struct powercoord{
	COORD powerlocation;
};

powercoord teleporter1location, teleporter2location, lifepoweruplocation, firepoweruplocation, flameslocation, freezepoweruplocation;

void initialisepowercoord()
{
	teleporter1location.powerlocation.X = 1;
	teleporter1location.powerlocation.Y = 1;
	teleporter2location.powerlocation.X = 1;
	teleporter2location.powerlocation.Y = 1;

	lifepoweruplocation.powerlocation.X = 1;
	lifepoweruplocation.powerlocation.Y = 1;

	firepoweruplocation.powerlocation.X = 1;
	firepoweruplocation.powerlocation.Y = 1;

	flameslocation.powerlocation.X = 1;
	flameslocation.powerlocation.Y = 1;

	freezepoweruplocation.powerlocation.X = 1;
	freezepoweruplocation.powerlocation.Y = 1;
}

struct playerhuman
{
	int health;
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
int enemyY[sizeY] = {25, 25, 25, 25, 25, 25, 25, 25};//Standard enemy Y COORD

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

EndScreen Gameover;

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

	// set the banana Coord and Status
	for(int i = 0; i<3; i++)
	{
		banana[i].active = true;
		banana[i].position.Y = 9;
	}
	banana[0].position.X = 20;
	banana[1].position.X = 40;
	banana[2].position.X = 60;

	// set the position of ladders
	initialiseLadders();

	// set enemy variables
	initialiseEnemy();

	//initialise endscreen
	Gameover.active = false;
	Gameover.type = 0;


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

struct activatepowerup{
	bool activated;
};

activatepowerup afire;


void activatepowerups(activatepowerup& state,bool activated)
{
	afire.activated = false;
}

struct randomlevelpos{
	int levelpos;
};

randomlevelpos lfire;


void setrandomlevelpos(randomlevelpos& random,int levelpos)
{
	//random fire level
	lfire.levelpos = 4;
}

struct timestamps{
	double timestamp;
	bool stamptime;
};

timestamps tele, life, fire, flame, freeze;

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

	//flame lifetime
	flame.timestamp = 0.0;
	flame.stamptime = true;

	//freeze powerup
	freeze.timestamp = 0.0;
	freeze.stamptime = true;
}

struct powerups{
	bool present;
};

powerups plife, pfire, pfreeze;


void setpowerups(powerups& spawning,bool present)
{
	//life powerup
	plife.present = false;

	//fire powerup
	pfire.present = false;

	//freeze powerup
	pfreeze.present = false;
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
				lifepoweruplocation.powerlocation.X = (n * 3) + 1;
			}
		}
		lifepoweruplocation.powerlocation.Y = (3);
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
		if(charLocation.X == lifepoweruplocation.powerlocation.X)
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

void firepowerup()
{
	bool getfire = true;//able to pick up power up
	int randomloop = rand() % 24 + 1;
	int randomnum = rand() % 3 + 1;

	//change random firepowerup location
	if(pfire.present== false)
	{
		for(int n = 0;n < randomloop;n++)
		{
			if(randomloop - 1 == n)
			{
				firepoweruplocation.powerlocation.X = (n * 3) + 1;
			}
		}
		firepoweruplocation.powerlocation.Y = (3);
	}

	//firepowerup random spawn time
	if(fire.stamptime == true && pfire.present == false)
	{
		fire.timestamp = elapsedTime;
		fire.stamptime = false;
	}
	if(elapsedTime > fire.timestamp + rand() % 10 + 5 && pfire.present == false)//change random spawn timing here
	{
		pfire.present = true;
		getfire = true;
		fire.stamptime = true;
	}

	//fire power up effect
	if(getfire == true && pfire.present == true)
	{
		if(charLocation.X == firepoweruplocation.powerlocation.X)
		{
			flameslocation.powerlocation.X = 0;
			switch(randomnum)
			{
			case 3: flameslocation.powerlocation.Y = 15;
				break;
			case 2: flameslocation.powerlocation.Y = 21;
				break;
			case 1: flameslocation.powerlocation.Y = 27;
			}
			//kill enemies
			for(int i = 0; i<6; i++)
			{
				if(enemyList[i].position.Y == flameslocation.powerlocation.Y - 1)
				{
					enemyList[i].health = 0;
				}
			}
			pfire.present = false;
			getfire = false;
			afire.activated = true;
			flame.timestamp = elapsedTime;
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
		teleporter1location.powerlocation.Y = (15);
		teleporter1location.powerlocation.
	X = (rand() % 76 + 1);

		//location for teleporter2
		teleporter2location.powerlocation.X = (rand() % 76 + 1);
		if(rand() % 2 + 1 == 2)
		{
			teleporter2location.powerlocation.Y = (21);
		}
		else
		{
			teleporter2location.powerlocation.Y = (27);
		}
		tele.stamptime = true;
	}

	//teleport enemies
	if(elapsedTime>rand() % 1 + 2)//time before teleporters spawn
	{
		//teleporter 1
		for(int i = 0; i<6; i++)
		{
			if(enemyList[i].position.Y == teleporter1location.powerlocation.Y - 2 && enemyList[0].position.X == teleporter1location.powerlocation.X)
			{
				if(telecd == false)
				{
					enemyList[i].position.Y = teleporter2location.powerlocation.Y-2;
					enemyList[i].position.X = teleporter2location.powerlocation.X;
					telecd = true;
				}
			}
		}
		//teleporter 2
		for(int i = 0; i<6; i++)
		{
			if(enemyList[i].position.Y == teleporter2location.powerlocation.Y - 2 && enemyList[i].position.X == teleporter2location.powerlocation.X)
			{
				if(telecd == false)
				{
					enemyList[i].position.Y = teleporter1location.powerlocation.Y-2;
					enemyList[i].position.X = teleporter1location.powerlocation.X;
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
		firepowerup();
		if(versus == true)
		{
			multiplayerdead();//gameover conditions for versus mode
		}
		else
		{
			monkeydead();//gameover conditions for single player
		}
		if(versus == true)
		{
			LoadMap(2);
		}
		else
		{
			LoadMap(1);
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

void render(int a)// for drawing of objects only
{
	if (pause == false && Gameover.active == false)
	{
		int print = 0;
		int printplayer = 0;
		char player[3][3] = {
			{' ','O',' '},
			{'-','|','-'},
			{'/',' ','\\'}
		};
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

		// render player human if in versus mode
		if(a == 2)
		{
			gotoXY(playerhumanLocation);
			colour(0x0E);
			for(int i = 0; i<=2; ++i)
			{
				for(int j = 0; j<=2; ++j)
				{
					std::cout << player[i][j];
				}
				printplayer++;
				gotoXY(playerhumanLocation.X,playerhumanLocation.Y+printplayer);
			}
		}
		// render barrel
		drawbarrel();

		//render teleporters
		if(elapsedTime>2)//time taken for teleporters to spawn
		{
			gotoXY(teleporter1location.powerlocation);
			colour(0x0C);
			std::cout << (char)5;
			std::cout << (char)5;
			gotoXY(teleporter2location.powerlocation);
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
				std::cout<<(char)247;
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
			gotoXY(lifepoweruplocation.powerlocation);
			colour(0x0C);
			std::cout<<(char)3;
		}

		//render fire power up
		if(pfire.present == true)
		{
			gotoXY(firepoweruplocation.powerlocation);
			colour(0x0C);
			std::cout<<(char)21;
		}

		//render flames of fire power up
		if(afire.activated == true)
		{
			gotoXY(flameslocation.powerlocation);
			colour(0x4E);//orange
			for(int n = 0; n < 79;n++)
			{
				std::cout<<(char)15;
			}
			std::cout<<std::endl;
			for(int n = 0; n < 79;n++)
			{
				std::cout<<(char)15;
			}
			if(elapsedTime > flame.timestamp + 1)//flame lifetime
			{
				afire.activated = false;
			}
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
	if(Gameover.active == true && Gameover.type == 1)
	{
		gotoXY(13,consoleSize.Y/4);
		colour(0x0E);
		cout<<"*******************************************************";
		for(int i = 8; i<14; i++)
		{
			gotoXY(13,i);
			colour(0x0E);
			cout<<"*                                                     *";
		}
		gotoXY(13,10);
		colour(0x0E);
		cout<<"*                Highest Score: ";
		printhighScore();
		cout<<"seconds             *"<<endl;
		gotoXY(13,12);
		colour(0x0E);
		cout<<"*      Congratulations!! Your Score is ";
		printScore(elapsedTime*1);
		cout<<"seconds      *"<<endl;
		gotoXY(13,consoleSize.Y/2);
		colour(0x0E);
		cout<<"*  You will return to the main menu in a short while  *";
		for(int i = 15; i<21; i++)
		{
			gotoXY(13,i);
			colour(0x0E);
			cout<<"*                                                     *";
		}
		gotoXY(13,21);
		colour(0x0E);
		cout<<"*******************************************************";
	}
	if(Gameover.active == true && Gameover.type == 2)
	{
		gotoXY(13,consoleSize.Y/4);
		colour(0x0E);
		cout<<"*******************************************************";
		for(int i = 8; i<14; i++)
		{
			gotoXY(13,i);
			colour(0x0E);
			cout<<"*                                                     *";
		}
		gotoXY(13,10);
		colour(0x0E);
		cout<<"*                 Player 2 Victory!                   *";
		gotoXY(13,consoleSize.Y/2);
		colour(0x0E);
		cout<<"*  You will return to the main menu in a short while  *";
		for(int i = 15; i<21; i++)
		{
			gotoXY(13,i);
			colour(0x0E);
			cout<<"*                                                     *";
		}
		gotoXY(13,21);
		colour(0x0E);
		cout<<"*******************************************************"<<endl;
	}
	if(Gameover.active == true && Gameover.type == 3)
	{
		gotoXY(13,consoleSize.Y/4);
		colour(0x0E);
		cout<<"*******************************************************";
		for(int i = 8; i<14; i++)
		{
			gotoXY(13,i);
			colour(0x0E);
			cout<<"*                                                     *";
		}
		gotoXY(13,10);
		colour(0x0E);
		cout<<"*                 Player 1 Victory!                   *";
		gotoXY(13,consoleSize.Y/2);
		colour(0x0E);
		cout<<"*  You will return to the main menu in a short while  *";
		for(int i = 15; i<21; i++)
		{
			gotoXY(13,i);
			colour(0x0E);
			cout<<"*                                                     *";
		}
		gotoXY(13,21);
		colour(0x0E);
		cout<<"*******************************************************"<<endl;
	}
	else if (pause == true)
	{
		gotoXY(20,0);
		cout<<"Game is Paused, Press BACKSPACE to continue";
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
	if(enemyList[identity].position.Y == 19)
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
	else if(enemyList[identity].position.Y == 13)
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
	else if(enemyList[identity].position.Y == 7)
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
		//If is climbing, cannot change to unable to climb
		else if(enemyList[identity].isClimbing == true)
		{
			return true;
		}
	}
	 //Only climb at ladders designated with '2' char in the array
	if(map[enemyList[identity].position.X+1][enemyList[identity].position.Y+2] == '2')
	{
		return true;
	}   
	else
	{
		//Unable to climb at not at a ladder
		return false;
	}
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

void monkeydead()
{
	int death = 0;
	for(int i = 0; i<enemies; i++)
	{
		COORD temp = enemyList[i].position;
		for(int i = 0; i<3; i++)
		{
			if(temp.X-1 == banana[i].position.X && temp.Y+2 == banana[i].position.Y 
				|| temp.X+1 == banana[i].position.X && temp.Y+2 == banana[i].position.Y&& banana[i].active == true)
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
		HighscoreCheck(elapsedTime*1);
		showgameover();
	}


}

void showgameover()
{
	cls();
	Gameover.active = true;
	Gameover.type = 1;
	render(1);
	Sleep(15000);
	gameStart();
}

void multiplayerdead()
{
	int death = 0;
	playerhuman player2;
	player2.health = 3;
	for( int i = 0; i <= 2; ++i)
	{
		if(playerhumanLocation.X == banana[i].position.X && playerhumanLocation.Y+2 == banana[i].position.Y && banana[i].active == true)
		{
			banana[i].active = false;
		}

	}
	for(int i = 0; i<enemies; i++)
	{
		COORD temp = enemyList[i].position;
		for(int i = 0; i<3; i++)
		{
			if(temp.X-1 == banana[i].position.X && temp.Y+2 == banana[i].position.Y 
				|| temp.X+1 == banana[i].position.X && temp.Y+2 == banana[i].position.Y&& banana[i].active == true)
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
		multiplayer1gameover();

	}
	for( int i = 0; i <= 2; ++i)
	{
		if(playerhumanLocation.X == barrellist[i].position.X && playerhumanLocation.Y == barrellist[i].position.Y)
		{
			player2.health--;
			multiplayer2gameover();
		}
	}
}

void multiplayer1gameover()
{
	cls();
	Gameover.active = true;
	Gameover.type = 2;
	render(1);
	Sleep(15000);
	gameStart();
}

void multiplayer2gameover()
{
	cls();
	Gameover.active = true;
	Gameover.type = 3;
	render(1);
	Sleep(15000);
	gameStart();
 }
