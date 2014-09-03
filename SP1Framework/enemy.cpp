#include "enemy.h"


extern char map[WIDTH][HEIGHT];//Using map data for char climbing
extern double dodgeTimer;//Dodge timer from update
extern double deltaTime;//delta time for resetting time based chances
extern COORD ConsoleSize;

//Kill counter to track score
int killCount = 0;

//Ascii for enemy humans
char enemy[3][3] = {
	{' ','O',' ',},
	{'-','|','-'},
	{'/',' ','\\'}
};

//Using defined array size
Enemy enemyList[maxEnemies];															//Enemy count
bool enemyType[maxEnemies] = {false, false, false, false, false, false, true, true};	//Enemy types
int enemyX[sizeX] = {25, 30, 0, 3, 6, 64, 67, 70};										//left enemy X=COORD
int enemyY[sizeY] = {25, 25, 25, 25, 25, 25, 25, 25};									//Standard enemy Y COORD

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
			//Move enemies if alive
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

//Drawing of enemies
void drawEnemy(int enemyCount)
{
	//Draws enemies based on current possible max count
	for(int i = 0; i < enemyCount; i++)
	{
		//Spawns enemies that are alive and not out of map
		if (enemyList[i].position.X >= 0 && enemyList[i].isAlive == true)
		{
			//Drawing at designated spawn point
			COORD temp = {enemyList[i].position.X,enemyList[i].position.Y};

			//Drawing of normal enemies
			if(enemyList[i].isSpecial == false)
			{
				for(int j = 0; j<=2; ++j)
				{
					for(int k = 0; k<=2; ++k)
					{
						writeToBuffer(temp, enemy[j][k], 0x08);
						temp.X += 1;					//Draws next char by incrementing X Coord
					}
					temp.Y+=1;							//Draws next line by incrementing Y Coord
					temp.X = enemyList[i].position.X;	//Resetting X Coord for next line
				}
			}

			//Drawing of special enemies
			else
				for(int j = 0; j<=2; ++j)
				{
					for(int k = 0; k<=2; ++k)
					{
						writeToBuffer(temp, enemy[j][k], 0x0F);
						temp.X += 1;					//Draws next char by incrementing X Coord
					}
					temp.Y+=1;							//Draws next line by incrementing Y Coord
					temp.X = enemyList[i].position.X;	//Resetting X Coord for next line
				}
		}	
	}
}

//Movement of enemies
void moveEnemy(int &enemy)
{
	//AI 1.0 Movement
	//Damage enemies based on available barrels
	for(int i = 0; i < barrelNum; i++)
	{
		//Collision against enemy bodies(X,X+1,X+2)
		if(enemyList[enemy].position.X == barrellist[i].position.X-1 && enemyList[enemy].position.Y == barrellist[i].position.Y
			|| enemyList[enemy].position.X == barrellist[i].position.X+1 && enemyList[enemy].position.Y == barrellist[i].position.Y
			|| enemyList[enemy].position.X == barrellist[i].position.X && enemyList[enemy].position.Y == barrellist[i].position.Y)
		{
			playGameSound(S_HIT);
			enemyList[enemy].health = 0;	//Sets collided enemies to 0 health
			killCount++;					//Increases kill count(score)
			break;
		}
	}
	//Only move if alive
	if(enemyList[enemy].health == 1)
	{
		//If allowed to move
		if(enemyList[enemy].canMove == true)
		{
			//Map Boundary
			//Enables movement to the left
			if(enemyList[enemy].position.X > 76)
			{
				enemyList[enemy].toRight = false;
			}

			//Enables movement to the right
			if(enemyList[enemy].position.X < 1)
			{
				enemyList[enemy].toRight = true;
			}

			//Starts moving right upon reaching end of map
			if(enemyList[enemy].toRight == true)
			{
				enemyList[enemy].position.X++;
			}
			//Stops moving right upon reaching end of map
			else
			{
				enemyList[enemy].position.X--;
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
				if(enemyList[enemy].dodgeChance < 3)
				{
					//Dodges to left if moving right
					if(enemyList[enemy].toRight == true)
					{
						enemyList[enemy].position.X-=4;				//Dodging left
						enemyList[enemy].dodgeChance = rollDice();	//Reroll dodge chance after dodging
					}
					//Dodges to right if moving left
					else
					{
						enemyList[enemy].position.X+=4;				//Dodging right
						enemyList[enemy].dodgeChance = rollDice();	//Reroll dodge chance after dodging
					}
				}
			}
		}
		//AI 1.2 Climbing
		enemyClimb(enemy);
	}
	//If enemy is dead
	else if(enemyList[enemy].health == 0)
	{
		enemyAlive(enemy);	//Sets alive conditions to false
	}

	//AI 1.3 Dodge chance reset every 1.5 seconds if no dodge has occured
	if(dodgeTimer > 1.5)
	{
		dodgeTimer = 0;									//Resets timer
		enemyList[enemy].dodgeChance = rollDice();		//Rerolls dodge chance
	}
}
//Climbing of enemies
void enemyClimb(int &enemy)
{
	//AI 1.2 Climbing
	//Check to climb only at ladders
	enemyList[enemy].canClimb = (climbCheck(enemy));
	
	//If enabled to climb, climbs at a 50% rate
	if(enemyList[enemy].canClimb == true && enemyList[enemy].randNum < 4)
	{
		//Starts climbing
		enemyList[enemy].position.Y--;
		enemyList[enemy].isClimbing = true;
		enemyList[enemy].canMove = false;

		//Check allignment to platform
		climbAlign(enemy);
	}

	//Reroll chance for climbing if unable to climb
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
		//After alignment
		enemyList[enemy].canMove = true;		//Enable movement
		enemyList[enemy].canClimb = false;		//Disable climbing(prevent glitch)
		enemyList[enemy].isClimbing = false;	//Set to not climbing state
		enemyList[enemy].isJumpingOff = false;	//Set to not jumping off state(special enemies)

		//Randomized movement after climbing
		if(enemyList[enemy].isClimbing == false)
		{
			enemyList[enemy].randNum = rollDice();

			//Moving to left
			if(enemyList[enemy].randNum < 4)
			{
				enemyList[enemy].toRight = false;
			}

			//Moving to right
			else if(enemyList[enemy].randNum > 3)
			{
				enemyList[enemy].toRight = true;
			}

		}
	}
}

bool climbCheck(int &enemy)
{
	//AI 1.3 Evasive climbing
	for(int i = 0; i < 3; i++)
	{
		//If is climbing, cannot change to unable to climb for normal enemies
		if(enemyList[enemy].isClimbing == true && enemyList[enemy].isSpecial == false)
		{
			return true;
		}

		//Does not climb if crates are falling(match X)(enemy Y is below barrel)
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
					enemyList[enemy].position.X++;			//Increasing X Coord(right)
					enemyList[enemy].position.Y++;			//Increasing Y Coord(down)
					enemyList[enemy].isJumpingOff = true;	//Set jumping off state to true
					climbAlign(enemy);						//Check allignment to platform
				}
				else
				{
					enemyList[enemy].position.X--;			//Decreasing X Coord(left)
					enemyList[enemy].position.Y++;			//Increasing Y Coord(down)
					enemyList[enemy].isJumpingOff = true;	//Set jumping off state to true
					climbAlign(enemy);						//Check allignment to platform
				}
			}
			return false;
		}
	}
	//Only climb at ladders designated with '2' char in the array(ladder) 
	//+1 in X due to X Coord starting from top left
	//+2 in Y due to Y Coord starting from top
	if(map[enemyList[enemy].position.X+1][enemyList[enemy].position.Y+2] == '2')
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
	//Set death statuses
	enemyList[enemy].canMove = false;		//Unable to move
	enemyList[enemy].canClimb = false;		//Unable to climb
	enemyList[enemy].isClimbing = false;	//Sets climbing state to false
	enemyList[enemy].isJumpingOff = false;	//Sets jumping off state to false(special enemies)
	enemyList[enemy].isAlive = false;		//Sets alive state to false

	//Returns is not alive status
	return enemyList[enemy].isAlive;		//Returns state of alive
}

void drawDeadEnemy(int &enemy)
{
	//Go to specific enemy coord
	COORD temp = {enemyList[enemy].position.X,enemyList[enemy].position.Y};

	//Clears up dead enemy
	for(int j = 0; j<=2; ++j)
	{
		for(int k = 0; k<=2; ++k)
		{
			writeToBuffer(temp, "", 0x0c);		//Draws empty space to simulate enemy gone
			temp.X += 1;						//Draws next char by incrementing X Coord
		}
		temp.Y+=1;								//Draws next line by incrementing Y Coord
		temp.X = enemyList[enemy].position.X;	//Resetting X Coord for next line
	}
}

void showKill()
{
	COORD killName ={0, 0};
	writeToBuffer(killName, "KillCount: ", 0x0E);
	COORD kill ={12, 0};
	string Str = static_cast<std::ostringstream*>(&(std::ostringstream()<<killCount))->str();
	writeToBuffer(kill, Str, 0x0E);
}