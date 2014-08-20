// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>

double elapsedTime;
double deltaTime;
bool keyPressed[K_COUNT];
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

	// set the banana at top of the ladders.
    life1.X = consoleSize.X / 2;
    life1.Y = 10;
	life2.X = 20;
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

    elapsedTime = 0.0;
}

void shutdown()
{
    // Reset to white text on black background
	colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void getInput()
{    
    keyPressed[K_UP] = isKeyPressed(VK_UP);
    keyPressed[K_DOWN] = isKeyPressed(VK_DOWN);
    keyPressed[K_LEFT] = isKeyPressed(VK_LEFT);
	keyPressed[K_RIGHT] = isKeyPressed(VK_RIGHT);
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
	int banana1 = 0;
	int banana2 = 0;
	int banana3 = 0;
	char player[3][3] = {
		{' ','O',' '},
		{'-','|','-'},
		{'/',' ','\\'}
	};

    // Updating the location of the character based on the key press
    if (keyPressed[K_UP] && charLocation.Y > 0)
    {
        Beep(1440, 30);
        charLocation.Y--; 
    }
    if (keyPressed[K_LEFT] && charLocation.X > 0)
    {
        Beep(1440, 30);
        charLocation.X--;
    }
    if (keyPressed[K_DOWN] && charLocation.Y < consoleSize.Y - 1)
    {
        Beep(1440, 30);
        charLocation.Y++; 
    }
    if (keyPressed[K_RIGHT] && charLocation.X < consoleSize.X - 1)
    {
        Beep(1440, 30);
        charLocation.X++;
    }
	
	//spawn life1
	if(banana1==0)
	{
		gotoXY(life1);
		colour(0x0E);
		std::cout<<"@@";
		banana1=1;
	}

	//spawn life2
	if(banana2==0)
	{
		gotoXY(life2);
		colour(0x0E);
		std::cout<<"@@";
		banana2=1;
	}

	//spawn life3
	if(banana3==0)
	{
		gotoXY(life3);
		colour(0x0E);
		std::cout<<"@@";
		banana3=1;
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

    // quits the game if player hits the escape key
    if (keyPressed[K_ESCAPE])
        g_quitGame = true;    
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

    
}
