#include "Levels.h"
#pragma once
int i;
int j;
int a ;
int b;
int Floor;
int Result;

void savelevel (void)
{
	ofstream myfile;
	myfile.open ("platform.txt", ios::in|ios::trunc);
	myfile.close();
}

void initFloors(void)
{
	Result=0;
	if (Floor == 0)
	{
		i=0;
		j=6;

	}
	else if (Floor == 1)
	{
		i=6;
		j=12;
	}
	else if	(Floor ==2)
	{
		i=12;
		j=18;
	}	
}

void loadplatforms (int y)
{

	string line;
	ifstream platform ("platform.txt");
	for (int l=0 ; l<5; l++)
	{
		if (platform.is_open())
		{
			getline (platform,line);
			stringstream convert(line);
			if ( !(convert >> Result) )//give the value to Result using the characters in the string
			{
			}
			int temp = y-Result;
			gotoXY(0,temp);
			DrawLevel();
		}
	}
	platform.close();

}

void loadlevel1 (int y)
{
loadplatforms(y);
	initFloors();
	string line;
	ifstream myfile ("Level1.txt");
	if (myfile.is_open())
	{
		for (Floor = 0 ; Floor < 3 ;Floor ++)
		{
			if (Floor == 0)
			{
				b=2;

			}
			else if (Floor == 1)
			{
				b=3;
			}
			else if	(Floor ==2)
			{
				b=2;
			}	

			for (a=0; a <b; a++)
			{

				getline (myfile,line);
				stringstream convert(line);
				if ( !(convert >> Result) )//give the value to Result using the characters in the string
				{
				}
				int x = Result;

				DrawLevel(x,y);

			}
		}

	}
	myfile.close();

}


void loadlevel2 (int y)
{
	loadplatforms(y);
	initFloors();
	string line;
	ifstream myfile ("Level2.txt");
	if (myfile.is_open())
	{
		for (Floor = 0 ; Floor < 3 ;Floor ++)
		{
			if (Floor == 0)
			{
				b=3;

			}
			else if (Floor == 1)
			{
				b=2;
			}
			else if	(Floor ==2)
			{
				b=3;
			}	

			for (a=0; a <b; a++)
			{

				getline (myfile,line);
				stringstream convert(line);
				if ( !(convert >> Result) )//give the value to Result using the characters in the string
				{
				}
				int x = Result;

				DrawLevel(x,y);

			}
		}

	}
	myfile.close();

}


void DrawLevel()
{
	colour(0x3C);
	std::cout << "                                                                                ";

}

void DrawLevel(int x, int y)
{
	initFloors();
	for(i ; i < j ; i++)
	{
		colour(0x4D);
		gotoXY(x,y-i);
		std::cout << "  "<<endl;
	}

}