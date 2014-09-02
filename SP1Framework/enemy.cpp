#include "enemy.h"


extern char map[WIDTH][HEIGHT];
extern double elapsedTime;
extern double dodgeTimer;
extern double deltaTime;
extern COORD ConsoleSize;


using std::cout;

int printLineX = 0;
int printLineY = 0;
int killCount = 0;
char enemy[3][3] = {
	{' ','O',' ',},
	{'-','|','-'},
	{'/',' ','\\'}
};

//Using defined array size
Enemy enemyList[maxEnemies];	//Enemy count
bool enemyType[maxEnemies] = {false, false, false, false, false, false, true, true};
int enemyX[sizeX] = {25, 30, 0, 3, 6, 64, 67, 70};//left enemy X=COORD
int enemyY[sizeY] = {25, 25, 25, 25, 25, 25, 25, 25};//Standard enemy Y COORD

// Initialise enemies
void initialiseEnemy(void)
{
	for(int i = 0; i < maxEnemies; i++)
	{	
		//initialising stats
		enemyList[i].health = 1;
		enemyList[i].randNum = rollDice();
		enemyList[i].dodgeChance = rollDice();
		enemyList[i].toRight = true;
		enemyList[i].canClimb = true;
		enemyList[i].isClimbing = false;
		enemyList[i].canMove = true;
		enemyList[i].isAlive = true;
		enemyList[i].isJumpingOff = false;
		enemyList[i].position.X = enemyX[i];
		enemyList[i].position.Y = enemyY[i];
		enemyList[i].isSpecial = enemyType[i];
		enemyList[i].respawnTimer = 0.3;
	}
 
}

//Activate enemies
void activateEnemy(int enemyCount)
{
	for(int i = 0; i < enemyCount; i++)
		{
			if(enemyList[i].isAlive == true)
			{
				moveEnemy(i);
			}
			//If enemy is not alive
			else if(enemyAlive(i) == false)
			{
				//Respawn timer
				enemyList[i].respawnTimer -= deltaTime;

				//If countdown to 0 and below, respawns
				if(enemyList[i].respawnTimer < 0)
				{
					//Restores the selected enemy and properties to alive status
					enemyList[i].health = 1;
					enemyList[i].canMove = true;
					enemyList[i].isAlive = true;
					enemyList[i].respawnTimer += 0.3;
					enemyList[i].position.X = enemyX[i];
					enemyList[i].position.Y = enemyY[i];
				}
			}
		}
}


//Rolling dice for chance to climb
int rollDice()
{
	int randNum = 0;
	for(int i = 1; i < 6; i++)
	{
		randNum = rand() %6 +1;
	}
	return randNum;
}

void drawenemy(int enemyCount)
{
	if(elapsedTime > 0)
	{
		for(int i = 0; i < enemyCount; i++)
		{
			//Spawns enemies after 4 seconds
			if (enemyList[i].position.X >= 0 && enemyList[i].isAlive == true)
			{
				//Designated spawn point

				//Colors enemy
				COORD temp = {enemyList[i].position.X,enemyList[i].position.Y};
				//Drawing of enemy
				if(enemyList[i].isSpecial == false)
				{
					for(int j = 0; j<=2; ++j)
					{
						for(int k = 0; k<=2; ++k)
						{
							writeToBuffer(temp, enemy[j][k], 0x08);
							temp.X += 1;
						}
						temp.Y+=1;
						temp.X = enemyList[i].position.X;
					}
				}
				else
					for(int j = 0; j<=2; ++j)
					{
						for(int k = 0; k<=2; ++k)
						{
							writeToBuffer(temp, enemy[j][k], 0x0F);
							temp.X += 1;
						}
						temp.Y+=1;
						temp.X = enemyList[i].position.X;
					}
				//Resetting Y Pos after draw
				printLineY=0;
			}	
		}
	}
}

void moveEnemy(int &enemy)
{
	//AI 1.0 Movement
	//Damage enemies
	for(int i = 0; i < 3; i++)
	{
		//Collision against enemy bodies
		if(enemyList[enemy].position.X == barrellist[i].position.X && enemyList[enemy].position.Y == barrellist[i].position.Y
			|| enemyList[enemy].position.X+1 == barrellist[i].position.X && enemyList[enemy].position.Y == barrellist[i].position.Y
			|| enemyList[enemy].position.X-1 == barrellist[i].position.X && enemyList[enemy].position.Y == barrellist[i].position.Y)
		{
			enemyList[enemy].health = 0;
			killCount++;
			break;
		}
	}
	//Only move if alive
	if(enemyList[enemy].health == 1)
	{
		//If allowed to move after climbing conditions
		if(enemyList[enemy].canMove == true)
		{

			//Allows moving to the LEFT side upon reaching the corner
			if(enemyList[enemy].position.X > 76)
			{
				enemyList[enemy].toRight = false;
			}

			//Allows moving to the RIGHT side upon reaching the corner
			if(enemyList[enemy].position.X < 1)
			{
				enemyList[enemy].toRight = true;
			}

			//Moving right side after reaching left corner
			if(enemyList[enemy].toRight == true)
			{
				enemyList[enemy].position.X+= EnemySpd;
			}
			//Moving left side after reaching right corner
			else
			{
				enemyList[enemy].position.X-= EnemySpd;
			}
		}
		//AI 1.1 Evasive movement(Dodge)
		for(int i = 0; i < 3; i++)
		{
			//Checks for: on same position, above and active barrels and not climbing
			if(enemyList[enemy].position.X == barrellist[i].position.X 
				&& barrellist[i].active == true 
				&& barrellist[i].position.Y < enemyList[enemy].position.Y 
				&& enemyList[enemy].isClimbing == false)
			{
				//Checks the random number for to dodge(50%, chance reset every frame)
				if(enemyList[enemy].dodgeChance < 4)
				{
					//Dodges to left if moving right
					if(enemyList[enemy].toRight == true)
					{
						enemyList[enemy].position.X-=4;
						enemyList[enemy].dodgeChance = rollDice();
					}
					//Dodges to right if moving left
					else
					{
						enemyList[enemy].position.X+=4;
						enemyList[enemy].dodgeChance = rollDice();
					}
				}
			}
		}
		//AI 1.2 Climbing
		enemyClimb(enemy);
	}
	else if(enemyList[enemy].health == 0)
	{
		enemyAlive(enemy); 
	}

	//AI 1.3 Dodge chance reset every 1.5 seconds
	if(dodgeTimer > 1.5)
	{
		dodgeTimer = 0;
		enemyList[enemy].dodgeChance = rollDice();
	}
}

void enemyClimb(int &enemy)
{
	//AI 1.2 Climbing
	//Climbing ladders only if collided at a 50% rate
	if(enemyList[enemy].randNum < 4)
	{
		enemyList[enemy].canClimb = (climbCheck(enemy));

		//If enabled to climb
		if(enemyList[enemy].canClimb == true)
		{
			//Climb only if alive
			if(enemyList[enemy].health == 1)
			{
				//Starts climbing
				enemyList[enemy].position.Y--;
				enemyList[enemy].isClimbing = true;
				enemyList[enemy].canMove = false;

				//Alligns to platform after climbing
				climbAlign(enemy);
			}
			//Enemy is no longer alive
			else if(enemyList[enemy].health == 0)
			{
				enemyAlive(enemy); 
			}
		}
	}
	//Reroll chance
	else
	{
		enemyList[enemy].randNum = rollDice();
	}
}

void climbAlign(int &enemy)
{
	//Checking if aligned to platform with char '1'(platform)
	if(map[enemyList[enemy].position.X][enemyList[enemy].position.Y+3] == '1')
	{
		//Enables movement after alignment
		enemyList[enemy].canMove = true;
		enemyList[enemy].canClimb = false;
		enemyList[enemy].isClimbing = false;
		enemyList[enemy].isJumpingOff = false;
		//Randomize numbers again
		if(enemyList[enemy].isClimbing == false)
		{
			enemyList[enemy].randNum = rollDice();

			if(enemyList[enemy].randNum < 4)
			{
				enemyList[enemy].toRight = false;
			}
			else if(enemyList[enemy].randNum > 3)
			{
				enemyList[enemy].toRight = true;
			}

		}
	}
}

bool climbCheck(int &enemy)
{
	int y = ConsoleSize.Y;

	//AI 1.3 Evasive climbing
	for(int i = 0; i < 3; i++)
	{
		//Does not climb if crates are falling(match X)(Y is below)
		if(barrellist[i].active == true 
 			&& barrellist[i].position.Y < enemyList[enemy].position.Y
			&& barrellist[i].position.X == enemyList[enemy].position.X
			|| enemyList[enemy].isJumpingOff == true)
		{
			//Special enemy can jump off and avoid dropping barrels while climbing
			if(map[enemyList[enemy].position.X+1][enemyList[enemy].position.Y+3] != '1'
				&& enemyList[enemy].isSpecial == true)
			{
				//Random chance of jumping off left or right
				if(enemyList[enemy].randNum < 4)
				{
					enemyList[enemy].position.X++;
					enemyList[enemy].position.Y++;
					enemyList[enemy].isJumpingOff = true;
					climbAlign(enemy);
				}
				else
				{
					enemyList[enemy].position.X--;
					enemyList[enemy].position.Y++;
					enemyList[enemy].isJumpingOff = true;
					climbAlign(enemy);
				}
			}
			
			return false;
		}
	}
	//Only climb at ladders designated with '2' char in the array(ladder) 
	if(map[enemyList[enemy].position.X+1][enemyList[enemy].position.Y+2] == '2')
	{
		return true;
	}	

	//If is climbing, cannot change to unable to climb
	if(enemyList[enemy].isClimbing == true)
	{
		return true;
	}

	//Unable to climb when not at ladders
	else
	{
		return false;
	}
}

bool enemyAlive(int &enemy)
{
	//Vertical Line for printing enemes

	//Disables movement
	enemyList[enemy].canMove = false;

	//draw dead enemy

	enemyList[enemy].isAlive = false;

	//Returns is not alive status
	return enemyList[enemy].isAlive;
}

void drawdeadEnemy(int &enemy)
{
	//Vertical Line for printing enemes
	int printLine = 0;

	//Go to specific enemy coord
	COORD temp = {enemyList[enemy].position.X,enemyList[enemy].position.Y};

	//Clears up dead enemy
	for(int j = 0; j<=2; ++j)
	{
		for(int k = 0; k<=2; ++k)
		{
			writeToBuffer(temp, " ", 0x0c);
			temp.X += 1;
		}
		temp.Y+=1;
		temp.X = enemyList[enemy].position.X;
		printLine++;
	}
}
