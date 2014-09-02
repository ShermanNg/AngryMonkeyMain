#include "LevelEditor.h"

void InitLevelEditor(void)
{
	CursorLocation.X=0;
	CursorLocation.Y=26;
}
void SaveLevel (void)
{
	std::ofstream myfile;
	myfile.open ("LevelEditor.txt", std::ios::in|std::ios::trunc);
	if (myfile.is_open())
	{
		for (int h=0;h<29;h++)
		{
			for (int w=0;w<80; w++)
			{
				if((map[w][h]=='0')||(map[w][h]=='1')||(map[w][h]=='2'))
				{
					myfile << map[w][h];
				}
				else
				{
					myfile <<'0';
				}
				myfile<<',';

			}
			myfile <<endl;
		}
	}
}

void drawcursor(void)
{
	COORD a = {CursorLocation.X, CursorLocation.Y};
	writeToBuffer(a, ' ', 0xff);
}
void ClearMap (void)
{
	for (int h=0;h<29;h++)
	{
		for (int w=0;w<80; w++)
		{
			map[w][h]='0';
		}
	}
}