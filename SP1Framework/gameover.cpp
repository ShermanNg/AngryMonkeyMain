#include "gameover.h"



extern double elapsedTime;
extern COORD consoleSize;
extern COORD charLocation;
extern COORD playerhumanLocation;
extern int killCount;

EndScreen Gameover;

void initialiseEnd()
{
	Gameover.active = false;
	Gameover.type = 0;
}
void drawNormalEnd()
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
	cout<<"kills               *"<<endl;
	gotoXY(13,12);
	colour(0x0E);
	cout<<"*      Congratulations!! Your Score is ";
	printScore(killCount);
	cout<<"kills"<<endl;
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

void drawplayer1Lose()
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

void drawplayer2Lose()
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
		HighscoreCheck(killCount);
		showgameover();
	}
}

void showgameover()
{
	cls();
	Gameover.active = true;
	Gameover.type = 1;
	resetpowerup();
	render();
	Sleep(5000);
	killCount = 0; //Reset killcount
	Gameover.active = false;
	init();
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
	resetpowerup();
	render();
	Sleep(5000);
	Gameover.active = false;
	init();
	gameStart();
}

void multiplayer2gameover()
{
	cls();
	Gameover.active = true;
	Gameover.type = 3;
	resetpowerup();
	render();
	Sleep(5000);
	Gameover.active = false;
	init();
	gameStart();
 }