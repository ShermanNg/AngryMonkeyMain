#include "gameover.h"

extern double elapsedTime;
extern COORD ConsoleSize;
extern COORD charLocation;
extern COORD playerhumanLocation;
extern int killCount;
extern int bananaNum;
extern bool godMode;

EndScreen Gameover;

void initialiseEnd()
{
	Gameover.active = false;
	Gameover.type = 0;
}
void drawNormalEnd()
{
	std::ifstream scoreScr;
	string scoreText;
	int y = 0;

	clearBuffer(0x0F);
	scoreScr.open("Text/Score.txt");
	while(!scoreScr.eof())
	{
		COORD score = {13,y+6};
		getline(scoreScr, scoreText);
		writeToBuffer(score,scoreText,0x0F);
		y++;
		
	}
	scoreScr.close();
	printhighScore();
	printScore(killCount);

}

void drawplayer1Lose()
{
	std::ifstream scoreScr;
	string scoreText;
	int y = 0;

	clearBuffer(0x0F);
	scoreScr.open("Text/player2win.txt");
	while(!scoreScr.eof())
	{
		COORD score = {13,y+6};
		getline(scoreScr, scoreText);
		writeToBuffer(score,scoreText,0x0F);
		y++;
	}
	scoreScr.close();
}

void drawplayer2Lose()
{
	std::ifstream scoreScr;
	string scoreText;
	int y = 0;

	clearBuffer(0x0F);
	scoreScr.open("Text/player1win.txt");
	while(!scoreScr.eof())
	{
		COORD score = {13,y+6};
		getline(scoreScr, scoreText);
		writeToBuffer(score,scoreText,0x0F);
		y++;
	}
	scoreScr.close();
}

void monkeydead()
 {
	 int death = 0;
	 for(int i = 0; i<maxEnemies; i++)
	 {
		 COORD temp = enemyList[i].position;
		 for(int j = 0; j<3; j++)
		 {
			 if(temp.X-1 == banana[j].position.X && temp.Y+2 == banana[j].position.Y 
				 || temp.X+1 == banana[j].position.X && temp.Y+2 == banana[j].position.Y && banana[j].active == true)
			 {
				 bananaNum--;
				 banana[j].active = false;
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
	Gameover.active = true;
	Gameover.type = 1;
	resetpowerup();
	

	mapload = false;
	for(int i = 0; i<barrelNum; i++)
	{
		barrellist[i].active = false;
		barrellist[i].position.X = charLocation.X;
		barrellist[i].position.Y = charLocation.Y+1;
	}
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
	for(int i = 0; i<maxEnemies; i++)
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
	Gameover.active = true;
	Gameover.type = 2;
	resetpowerup();
	mapload = false;
	for(int i = 0; i<barrelNum; i++)
	{
		barrellist[i].active = false;
		barrellist[i].position.X = charLocation.X;
		barrellist[i].position.Y = charLocation.Y+1;
	}
}

void multiplayer2gameover()
{
	Gameover.active = true;
	Gameover.type = 3;
	resetpowerup();
	mapload = false;
	for(int i = 0; i<barrelNum; i++)
	{
		barrellist[i].active = false;
		barrellist[i].position.X = charLocation.X;
		barrellist[i].position.Y = charLocation.Y+1;
	}
}