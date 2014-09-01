#include "HighScore.h"
#include "game.h"
# pragma once

int highscore;
int Score;
void HighscoreSave ()
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
		if (!(convert >> highscore) )//give the value to Result using the characters in the string
			return highscore;

		myfile.close();
	}
}

void HighscoreCheck(double b)
{
	Score = b;
	if (highscore < b)
	{
		HighscoreSave();
		highscore = b;
	}

}

void printhighScore (void)
{
	int c = highscore;
	COORD b = {42,12};
	string Str = static_cast<std::ostringstream*>(&(std::ostringstream()<<highscore))->str();
	writeToBuffer(b, Str, 0x0F);
}

void printScore(int a)
{
	COORD b = {55,13};
	string Str = static_cast<std::ostringstream*>(&(std::ostringstream()<<a))->str();
	writeToBuffer(b, Str, 0x0F);
}