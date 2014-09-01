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
	COORD temp = charLocation;
	/*gotoXY(charLocation);
	colour(0x0C);*/
	for(int i = 0; i<=4; ++i)
	{
		for(int j = 0; j<=6; ++j)
		{
			/*std::cout << player1[i][j];*/
			writeToBuffer(temp, player1[i][j], 0x0C);
			temp.X += 1;
		}
		temp.Y+=1;
		temp.X = charLocation.X;
		/*print++;*/
		/*gotoXY(charLocation.X,charLocation.Y+print);*/
	}
	print = 0;
}
void drawPlayer2(void)
{
	int printplayer = 0;
	COORD temp = playerhumanLocation;
	/*gotoXY(playerhumanLocation);
	colour(0x0E);*/
	for(int i = 0; i<=2; ++i)
	{
		for(int j = 0; j<=2; ++j)
		{
			/*std::cout << player2[i][j];*/
			writeToBuffer(temp, player2[i][j], 0x0E);
			temp.X += 1;
		}
		temp.Y+=1;
		temp.X = playerhumanLocation.X;
		/*printplayer++;
		gotoXY(playerhumanLocation.X,playerhumanLocation.Y+printplayer);*/
	}
	printplayer = 0;
}