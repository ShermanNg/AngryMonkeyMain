#include "Level.h"

# pragma once
char map[WIDTH][HEIGHT];

void save (void)
{
	std::ofstream myfile;
	myfile.open ("Level1.txt", std::ios::in|std::ios::trunc);
	myfile.close();
}

void LoadMap()
{
	string line;
	char *pch;
	char * str;
	if((versus == false) && (editor== false))
	{
		std::ifstream Level ("LevelEditor.txt");
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
				delete[] str;
			}
			else
				std::cout << "Error Loading Map"<<endl;
		}

	}
	if((versus == true)&&(editor==false))
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
			delete[] str;
		}

	}
	if((editor == true)&&(versus==false))
	{
		std::ifstream Level ("LevelEditor.txt");
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
	//Set X and Y for drawing map
	int x = 0;
	int y = 0;
	for (int i=0;i<29;i++)
	{	
		for(int j =0;j<80;j++)
		{
			symbol = map[j][i];
			DrawMap(symbol,x,y);
			x++;
		}
		//Resetting X for new line
		x = 0;
		y++;
	}
}

void DrawMap (char Sym,int X, int Y)
{
	//if (Sym == '1')
	//{
	//	colour(0x3C);
	//	cout <<" ";
	//}
	//else if (Sym == '2')
	//{
	//	colour(0x4D);
	//	cout << "H";
	//}
	//else if (Sym =='0')
	//{
	//	colour(0x00);
	//	cout <<" ";
	//}

	COORD draw = {X,Y};
	if(Sym == '0')
	{
		writeToBuffer(draw,' ',0x00);
	}
	else if(Sym == '1')
	{
		writeToBuffer(draw,' ',0x3C);
	}
	else if(Sym == '2')
	{
		writeToBuffer(draw,'H',0x0E);
	}

	/*else 
	{
	cout << " skip" ;
	}*/
}

void reloadlevel(void)
{
		string line;
	char *pch;
	char * str;
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
			delete[] str;
		}
		else
			std::cout << "Error Loading Map"<<endl;
	}

	SaveLevel ();
}