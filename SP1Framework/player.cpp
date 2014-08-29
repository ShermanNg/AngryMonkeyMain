#include "player.h"

extern COORD charLocation;
extern COORD playerhumanLocation;



char player1[5][7] = {
	{' ',' ','_','_','_',' ',' '},
	{'[','|','O',' ','O','|',']'},
	{' ',' ','|','-','|',' ',' '},
	{'/','(','(','_',')',')','\\'},
	{' ','/',' ',' ',' ','\\',' '}
};

char player2[3][3] = {
	{' ','O',' '},
	{'-','|','-'},
	{'/',' ','\\'}
};

void drawPlayer1(void)
{
	int print = 0;
	gotoXY(charLocation);
	colour(0x0C);
	for(int i = 0; i<=4; ++i)
	{
		for(int j = 0; j<=6; ++j)
		{
			std::cout << player1[i][j];
		}
		print++;
		gotoXY(charLocation.X,charLocation.Y+print);
	}
	print = 0;
}
void drawPlayer2(void)
{
	int printplayer = 0;
	gotoXY(playerhumanLocation);
	colour(0x0E);
	for(int i = 0; i<=2; ++i)
	{
		for(int j = 0; j<=2; ++j)
		{
			std::cout << player2[i][j];
		}
		printplayer++;
		gotoXY(playerhumanLocation.X,playerhumanLocation.Y+printplayer);
	}
	printplayer = 0;
}