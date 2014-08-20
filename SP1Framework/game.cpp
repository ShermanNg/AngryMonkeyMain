// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>

double elapsedTime;
double deltaTime;
double teleportertimestamp;
double lifetimestamp;
bool keyPressed[K_COUNT];
bool telecd = false;// teleporter cooldown
bool teleporterstamptime = true;
bool spawn = false;
bool banana1=true, banana2=false, banana3=false;//bananas(life)
bool lifepowerup = true;//power ups
bool getpowerup = true;//able tp pick up power up
bool lifestamptime = true;
COORD charLocation;
COORD lefthumanLocation;
COORD lefthuman2Location;
COORD lefthuman3Location;
COORD righthumanLocation;
COORD righthuman2Location;
COORD righthuman3Location;
COORD life1;
COORD life2;
COORD life3;
COORD consoleSize;
COORD teleporter1Location;
COORD teleporter2Location;
COORD barrelLocation;
barrel barrelhitting[10];
COORD lifepowerupLocation;

void init()
{
    // Set precision for floating point output
    std::cout << std::fixed << std::setprecision(3);

    SetConsoleTitle(L"SP1 Framework");

    // Sets the console size, this is the biggest so far.
    setConsoleSize(79, 28);

    // Get console width and height
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */     

    /* get the number of character cells in the current buffer */ 
    GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );
    consoleSize.X = csbi.srWindow.Right + 1;
    consoleSize.Y = csbi.srWindow.Bottom + 1;

    // set the character to be in the center of the screen.
    charLocation.X = consoleSize.X / 2;
    charLocation.Y = 2;

	// set barrel coord
	barrelLocation.X = charLocation.X;
	barrelLocation.Y = charLocation.Y+1;

	// set the banana at top of the ladders.
    life1.X = 20;
    life1.Y = 10;
	life2.X = consoleSize.X / 2;
    life2.Y = 10;
	life3.X = 60;
    life3.Y = 10;

	// set the human enemy to be at the bottom left of the screen
	lefthumanLocation.X = 0;
	lefthumanLocation.Y = 26;
	lefthuman2Location.X = 0;
	lefthuman2Location.Y = 20;
	lefthuman3Location.X = 0;
	lefthuman3Location.Y = 14;
	

	// set the human enemy to be at the bottom right of the screen
	righthumanLocation.X = 76;
	righthumanLocation.Y = 26;
	righthuman2Location.X = 76;
	righthuman2Location.Y = 20;
	righthuman3Location.X = 76;
	righthuman3Location.Y = 14;

	//teleporter
	 teleporter1Location.X = (49);
	 teleporter1Location.Y = (16);
	 teleporter2Location.X = (20);
	 teleporter2Location.Y = (22);

	//location for life power up
	lifepowerupLocation.X = (rand() % 72 + 3);
	lifepowerupLocation.Y = (4);

    elapsedTime = 0.0;
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
}

void update(double dt)
{
    // get the delta time
    elapsedTime += dt;
    deltaTime = dt;
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
    if (keyPressed[K_LEFT] && charLocation.X > 0)
    {
        Beep(1440, 30);
        charLocation.X--;
		if(spawn == false)
		{
			barrelLocation.X--;
		}
    }
    if (keyPressed[K_RIGHT] && charLocation.X < consoleSize.X - 3)
    {
        Beep(1440, 30);
        charLocation.X++;
		if(spawn == false)
		{
			barrelLocation.X++;
		}
    }
	if(keyPressed[K_SPACE])
	{
		drawbarrel();
		barrelshooting();
	}

	if (spawn == true)
	{
		drawbarrel();

		if(barrelLocation.Y >= 27)
		{
			barrelLocation.Y = 28;
		}
		else
		{
			barrelLocation.Y++;
		}                                                                                
		if(barrelLocation.Y == consoleSize.Y-1)
		{
			barrelLocation.X = charLocation.X;
			barrelLocation.Y = charLocation.Y+1;
			spawn = false;
		}
	}

	// first enemy spawn at bottom left
	if(leftNum == 0)
	{
		gotoXY(lefthumanLocation);
		colour(0x0c);
		for(int i = 0; i<=2; ++i)
		{
			for(int j = 0; j<=2; ++j)
			{
				std::cout << player[i][j];
			}
			printL++;
			gotoXY(lefthumanLocation.X,lefthumanLocation.Y+printL);
		}
		leftNum++;
		printL=0;
	}
	//updating the location of first left spawn enemy based on time
	if (elapsedTime>2 && lefthumanLocation.X>=0)
	{
		Beep(0, 30);
		lefthumanLocation.X++;
	}

	// 2nd enemy spawn at bottom left
	if(elapsedTime>4 && leftNum==1)
	{
		gotoXY(lefthuman2Location);
		colour(0x0c);
		for(int i = 0; i<=2; ++i)
		{
			for(int j = 0; j<=2; ++j)
			{
				std::cout << player[i][j];
			}
			printL++;
			gotoXY(lefthuman2Location.X,lefthuman2Location.Y+printL);
		}
		leftNum++;
		printL=0;
	}

	//updating the location of second left spawn enemy based on time
	if (elapsedTime>5 && lefthuman2Location.X>=0)
	{
		Beep(0, 30);
		lefthuman2Location.X++;
	}

	// 3rd enemy spawn at bottom left
	if(elapsedTime>6 && leftNum==2)
	{
		gotoXY(lefthuman3Location);
		colour(0x0c);
		for(int i = 0; i<=2; ++i)
		{
			for(int j = 0; j<=2; ++j)
			{
				std::cout << player[i][j];
			}
			printL++;
			gotoXY(lefthuman3Location.X,lefthuman3Location.Y+printL);
		}
		leftNum++;
		printL=0;
	}
	//updating the location of third left spawn enemy based on time
	if (elapsedTime>7 && lefthuman3Location.X>=0)
	{
		Beep(0, 30);
		lefthuman3Location.X++;
	}

    // first enemy spawn at bottom right
	if(rightNum==0)
	{
	gotoXY(righthumanLocation);
	colour(0x0c);
	for(int i = 0; i<=2; ++i)
	{
		for(int j = 0; j<=2; ++j)
		{
			std::cout << player[i][j];
		}
		printR++;
		gotoXY(righthumanLocation.X,righthumanLocation.Y+printR);
	}
	rightNum++;
	printR=0;
	}
	//updating the location of right spawn enemy based on time
	if (elapsedTime>2 && righthumanLocation.X>=0)
	{
		Beep(0, 30);
		righthumanLocation.X--;
	}

	// 2nd enemy spawn at bottom right
	if(elapsedTime>4 && rightNum==1)
	{
		gotoXY(righthuman2Location);
		colour(0x0c);
		for(int i = 0; i<=2; ++i)
		{
			for(int j = 0; j<=2; ++j)
			{
				std::cout << player[i][j];
			}
			printR++;
			gotoXY(righthuman2Location.X,righthuman2Location.Y+printR);
		}
		rightNum++;
		printR=0;
	}
	//updating the location of second right spawn enemy based on time
	if (elapsedTime>5 && righthuman2Location.X>=0)
	{
		Beep(0, 30);
		righthuman2Location.X--;
	}

	// 3rd enemy spawn at bottom right
	if(elapsedTime>6 && rightNum==2)
	{
		gotoXY(righthuman3Location);
		colour(0x0c);
		for(int i = 0; i<=2; ++i)
		{
			for(int j = 0; j<=2; ++j)
			{
				std::cout << player[i][j];
			}
			printR++;
			gotoXY(righthuman3Location.X,righthuman3Location.Y+printR);
		}
		rightNum++;
		printR=0;
	}
	//updating the location of 3rd right spawn enemy based on time
	if (elapsedTime>7 && righthuman3Location.X>=0)
	{
		Beep(0, 30);
		righthuman3Location.X--;
	}
	//update for tele
	 if(teleporterstamptime == true)
	 {
		 teleportertimestamp = elapsedTime;
		 teleporterstamptime = false;
	 }
	 if(elapsedTime > teleportertimestamp + 5)//change in teleporter location timing
	 {
		//location for teleporter1
		teleporter1Location.Y = (16);
		if(rand() % 2 + 1 == 2)
			 {
				 if(rand() % 2 + 1 == 2)
				 {
					 teleporter1Location.X = (rand() % 17 + 3);//2nd most left
				 }
				 else
				 {
					 teleporter1Location.X = (rand() % 16 + 22);//2nd 2 most left
				 }
			 }
			 else
			 {
				 if(rand() % 2 + 1 == 2)
				 {
					 teleporter1Location.X = (rand() % 17 + 42);//2nd 2 most right
				 }
				 else
				 {
					 teleporter1Location.X = (rand() % 14 + 62);//2nd most right
				 }
			 }
		 //location for teleporter2
		 if(rand() % 2 + 1 == 2)
		 {
			 teleporter2Location.Y = (22);
			 if(rand() % 2 + 1 == 2)
			 {
				 if(rand() % 2 + 1 == 2)
				 {
					 teleporter2Location.X = (rand() % 17 + 3);//3rd most left
				 }
				 else
				 {
					 teleporter2Location.X = (rand() % 16 + 22);//3rd 2 most left
				 }
			 }
			 else
			 {
				 if(rand() % 2 + 1 == 2)
				 {
					 teleporter2Location.X = (rand() % 17 + 42);//3rd 2 most right
				 }
				 else
				 {
					 teleporter2Location.X = (rand() % 14 + 62);//3rd most right
				 }
			 }
		 }
		 else
		 {
			 teleporter2Location.Y = (28);
			 if(rand() % 3 + 1 == 3)
			 {
				 teleporter2Location.X =(rand() % 26 + 3);//most left 4th lane
			 }
			 else
			 {
				 if(rand() % 2 + 1 == 2)
				 {
					 teleporter2Location.X = (rand() % 14 + 32);//middle 4th lane
				 }
				 else
				 {
					 teleporter2Location.X = (rand() % 14 + 53);//most right 4th lane
				 }
			 }
		 }
		 teleporterstamptime = true;
	 }
	 if(elapsedTime>2)//time before teleporters spawn
	 {
		 //left humans
		 //left human 1
		 //teleporter 1
		 if(lefthumanLocation.Y == teleporter1Location.Y - 2 && lefthumanLocation.X == teleporter1Location.X)
		 {
			 if(telecd == false)
			 {
				 lefthumanLocation.Y = teleporter2Location.Y-2;
				 lefthumanLocation.X = teleporter2Location.X;
				 telecd = true;
			 }
		 }
		 //teleporter 2
		 if(lefthumanLocation.Y == teleporter2Location.Y - 2&& lefthumanLocation.X == teleporter2Location.X)
		 {
			 if(telecd == false)
			 {
				 lefthumanLocation.Y = teleporter1Location.Y-2;
				 lefthumanLocation.X = teleporter1Location.X;
				 telecd = true;
			 }
		 }
		 //left human 2
		 //teleporter 1
		 if(lefthuman2Location.Y == teleporter1Location.Y -2 && lefthuman2Location.X == teleporter1Location.X)
		 {
			 if(telecd == false)
			 {
				 lefthuman2Location.Y = teleporter2Location.Y-2;
				 lefthuman2Location.X = teleporter2Location.X;
				 telecd = true;
			 }
		 }
		 //teleporter 2
		 if(lefthuman2Location.Y == teleporter2Location.Y  -2&& lefthuman2Location.X == teleporter2Location.X)
		 {
			 if(telecd == false)
			 {
				 lefthuman2Location.Y = teleporter1Location.Y-2;
				 lefthuman2Location.X = teleporter1Location.X;
				 telecd = true;
			 }
		 }
		 //left human 3
		 //teleporter 1
		 if(lefthuman3Location.Y == teleporter1Location.Y-2 && lefthuman3Location.X == teleporter1Location.X)
		 {
			 if(telecd == false)
			 {
				 lefthuman3Location.Y = teleporter2Location.Y-2;
				 lefthuman3Location.X = teleporter2Location.X;
				 telecd = true;
			 }
		 }
		 //teleporter 2
		 if(lefthuman3Location.Y == teleporter2Location.Y-2 && lefthuman3Location.X == teleporter2Location.X)
		 {
			 if(telecd == false)
			 {
				 lefthuman3Location.Y = teleporter1Location.Y-2;
				 lefthuman3Location.X = teleporter1Location.X;
				 telecd = true;
			 }
		 }
		 //right humans
		 //right human 1
		 //teleporter 1
		 if(righthumanLocation.Y == teleporter1Location.Y - 2 && righthumanLocation.X == teleporter1Location.X)
		 {
			 if(telecd == false)
			 {
				 righthumanLocation.Y = teleporter2Location.Y-2;
				 righthumanLocation.X = teleporter2Location.X;
				 telecd = true;
			 }
		 }
		 //teleporter 2
		 if(righthumanLocation.Y == teleporter2Location.Y - 2&& righthumanLocation.X == teleporter2Location.X)
		 {
			 if(telecd == false)
			 {
				 righthumanLocation.Y = teleporter1Location.Y-2;
				 righthumanLocation.X = teleporter1Location.X;
				 telecd = true;
			 }
		 }
		 //right human 2
		 //teleporter 1
		 if(righthuman2Location.Y == teleporter1Location.Y - 2 && righthuman2Location.X == teleporter1Location.X)
		 {
			 if(telecd == false)
			 {
				 righthuman2Location.Y = teleporter2Location.Y-2;
				 righthuman2Location.X = teleporter2Location.X;
				 telecd = true;
			 }
		 }
		 //teleporter 2
		 if(righthuman2Location.Y == teleporter2Location.Y - 2&& righthuman2Location.X == teleporter2Location.X)
		 {
			 if(telecd == false)
			 {
				 righthuman2Location.Y = teleporter1Location.Y-2;
				 righthuman2Location.X = teleporter1Location.X;
				 telecd = true;
			 }
		 }
		 //right human 3
		 //teleporter 1
		 if(righthuman3Location.Y == teleporter1Location.Y - 2 && righthuman3Location.X == teleporter1Location.X)
		 {
			 if(telecd == false)
			 {
				 righthuman3Location.Y = teleporter2Location.Y-2;
				 righthuman3Location.X = teleporter2Location.X;
				 telecd = true;
			 }
		 }
		 //teleporter 2
		 if(righthuman3Location.Y == teleporter2Location.Y - 2&& righthuman3Location.X == teleporter2Location.X)
		 {
			 if(telecd == false)
			 {
				 righthuman3Location.Y = teleporter1Location.Y-2;
				 righthuman3Location.X = teleporter1Location.X;
				 telecd = true;
			 }
		 }
	}
	telecd = false;
	//end of teleporter

    // quits the game if player hits the escape key
    if (keyPressed[K_ESCAPE])
        g_quitGame = true;  

	//random lifepowerup location
	if(lifepowerup == false)
	{
		lifepowerupLocation.X = (rand() % 72 + 3);
		lifepowerupLocation.Y = (4);
	}
	//lifepowerup random spawn
	if(lifestamptime == true && lifepowerup == false)
	{
		lifetimestamp = elapsedTime;
		lifestamptime = false;
	}
	if(elapsedTime > lifetimestamp + rand() % 10 + 5 && lifepowerup == false)//change spawn timing here
	{
		lifepowerup = true;
		getpowerup = true;
		lifestamptime = true;
	}

	//lifepowerup
	if(getpowerup == true)
	{
		if(charLocation.X == lifepowerupLocation.X)
		{
			if(banana2 == false)
			{
				banana2 = true;
			}
			else
			{
				if(banana3 == false)
				{
					banana3 = true;
				}
			}
			lifepowerup = false;
			getpowerup = false;
		}
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
    //render lives
	//spawn life1
	if(banana1==true)
	{
		gotoXY(life1);
		colour(0x0E);
		std::cout<<"@@";
		banana1=1;
	}
	//spawn life2
	if(banana2==true)
	{
		gotoXY(life2);
		colour(0x0E);
		std::cout<<"@@";
		banana2=1;
	}
	//spawn life3
	if(banana3==true)
	{
		gotoXY(life3);
		colour(0x0E);
		std::cout<<"@@";
		banana3=1;
	}

	//render life power up
	if(lifepowerup == true)
	{
		gotoXY(lifepowerupLocation);
		colour(0x0C);
		std::cout<<(char)3;
	}
}

void barrelshooting()
{
	barrel*barrelone = new barrel;
	barrelone->x = barrelLocation.X;
	spawn = true;
}

void drawbarrel()
{
	gotoXY(barrelLocation);
	colour(0xA2);
	std::cout<<(char)4;
}
