#include "gameover.h"

extern double elapsedTime;
extern COORD ConsoleSize;
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
	COORD a = {(13,ConsoleSize.Y/4)};
	std::ifstream scoreScr;
	string scoreText;
	int y = 0;

	clearBuffer(0x0F);
	scoreScr.open("Text/Score.txt");
	while(!scoreScr.eof())
	{
		COORD score = {0,y};
		getline(scoreScr, scoreText);
		writeToBuffer(score,scoreText,0x0F);
		y++;
		
	}
	scoreScr.close();
	
	//Clear all this crap and make the end screen in a txt file for single,and multi(p1 win and p2 win)

	//writeToBuffer(a, "*******************************************************", 0x0E);
	//for(int i = 8; i<14; i++)
	//{
	//	COORD b = {(13,i)};

	//	writeToBuffer(b, "*                                                     *", 0x0E);
	//}
	//COORD c = {(13,10)};

	//writeToBuffer(c, "*                Highest Score: ", 0x0E);
	//printhighScore();
	////cout<<"kills               *"<<endl;*///fix later
	//COORD d = {(13,12)};

	//writeToBuffer(d, "*      Congratulations!! Your Score is ", 0x0E);
	//printScore(killCount);
	////cout<<"kills"<<endl;*///fix later
	//COORD e = {(13,ConsoleSize.Y/2)};

	//writeToBuffer(e, "*  You will return to the main menu in a short while  *", 0x0E);
	//for(int i = 15; i<21; i++)
	//{
	//	COORD f = {(13,i)};

	//	writeToBuffer(f, "*                                                     *", 0x0E);
	//}
	//COORD g = {13, 21};

	//writeToBuffer(g, "*******************************************************", 0x0E);

}

void drawplayer1Lose()
{
	gotoXY(13,ConsoleSize.Y/4);
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
	gotoXY(13,ConsoleSize.Y/2);
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
	gotoXY(13,ConsoleSize.Y/4);
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
	gotoXY(13,ConsoleSize.Y/2);
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
	mapload = false;
	for(int i = 0; i<barrelNum; i++)
	{
		barrellist[i].active = false;
		barrellist[i].position.X = charLocation.X;
		barrellist[i].position.Y = charLocation.Y+1;
	}
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
	mapload = false;
	for(int i = 0; i<barrelNum; i++)
	{
		barrellist[i].active = false;
		barrellist[i].position.X = charLocation.X;
		barrellist[i].position.Y = charLocation.Y+1;
	}
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
	mapload = false;
	for(int i = 0; i<barrelNum; i++)
	{
		barrellist[i].active = false;
		barrellist[i].position.X = charLocation.X;
		barrellist[i].position.Y = charLocation.Y+1;
	}
	gameStart();
 }