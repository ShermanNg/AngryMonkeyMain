#include "Level.h"

# pragma once
extern char map[WIDTH][HEIGHT];


void save (void)
{
	std::ofstream myfile;
	myfile.open ("Level1.txt", std::ios::in|std::ios::trunc);
	myfile.close();
}

void LoadMap(int a)
{
	string line;
	char *pch;
	char * str;
	if(a == 1)
	{
		std::ifstream Level ("Level1.txt");
		for (int i=0;i<29;i++)
	{	
		if (Level.is_open())
		{
			getline(Level,line);
			str = new char [line.length()+1];
			pch = strtok (str," ,");
			strcpy (str, line.c_str());

			int j =0;
			char * p = strtok (str," ,");
			while (p !=0)
			{
				map[j][i] = *p;
				p = strtok(NULL," ,");
				j++;
			}
		}
		else
			std::cout << "Error Loading Map"<<endl;
	}
	delete[] str;
	}
	if(a == 2)
	{
		std::ifstream Level ("Level2.txt");
		for (int i=0;i<29;i++)
	{	
		if (Level.is_open())
		{
			getline(Level,line);
			str = new char [line.length()+1];
			pch = strtok (str," ,");
			strcpy (str, line.c_str());

			int j =0;
			char * p = strtok (str," ,");
			while (p !=0)
			{
				map[j][i] = *p;
				p = strtok(NULL," ,");
				j++;
			}
		}
		else
			cout << "Error Loading Map"<<endl;
	}
	delete[] str;
	}
}

void read(void)
{
	char symbol = '0';

	for (int i=0;i<29;i++)
	{	
		for(int j =0;j<80;j++)
		{
			symbol = map[j][i] ;
			DrawMap(symbol);
		}
	}
}

void DrawMap (char Sym)
{
	if (Sym == '1')
	{
		colour(0x3C);
		cout <<" ";
	}
	else if (Sym == '2')
	{
		colour(0x4D);
		cout << "H";
	}
	else if (Sym =='0')
	{
		colour(0x00);
		cout <<" ";
	}
	/*else 
	{
		cout << " skip" ;
	}*/
}