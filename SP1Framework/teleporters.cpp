#include"teleporters.h"

extern double elapsedTime;
extern COORD charLocation;
extern COORD playerhumanLocation;


powercoord teleporter1location, teleporter2location, lifepoweruplocation, firepoweruplocation, flameslocation, freezepoweruplocation;
activatepowerup afire;
randomlevelpos lfire;
powerups plife, pfire, pfreeze;

timestamps tele, life, fire, flame, freeze;
void initialisepowercoord()
{
	teleporter1location.powerlocation.X = 1;
	teleporter1location.powerlocation.Y = 1;
	teleporter2location.powerlocation.X = 1;
	teleporter2location.powerlocation.Y = 1;

	lifepoweruplocation.powerlocation.X = 1;
	lifepoweruplocation.powerlocation.Y = 1;

	firepoweruplocation.powerlocation.X = 1;
	firepoweruplocation.powerlocation.Y = 1;

	flameslocation.powerlocation.X = 1;
	flameslocation.powerlocation.Y = 1;

	freezepoweruplocation.powerlocation.X = 1;
	freezepoweruplocation.powerlocation.Y = 1;
}

void teleporters()
{
	bool telecd = false;// teleporter cooldown

	if(tele.stamptime == true)
	{
		tele.timestamp = elapsedTime;
		tele.stamptime = false;
	}

	//change in teleporter location timing
	if(elapsedTime > tele.timestamp + 5)
	{
		//location for teleporter1
		teleporter1location.powerlocation.Y = (15);
		teleporter1location.powerlocation.
	X = (rand() % 76 + 1);

		//location for teleporter2
		teleporter2location.powerlocation.X = (rand() % 76 + 1);
		if(rand() % 2 + 1 == 2)
		{
			teleporter2location.powerlocation.Y = (21);
		}
		else
		{
			teleporter2location.powerlocation.Y = (27);
		}
		tele.stamptime = true;
	}

	//teleport enemies
	if(elapsedTime>rand() % 1 + 2)//time before teleporters spawn
	{
		//teleporter 1
		for(int i = 0; i<6; i++)
		{
			if(enemyList[i].position.Y == teleporter1location.powerlocation.Y - 2 && enemyList[0].position.X == teleporter1location.powerlocation.X)
			{
				if(telecd == false)
				{
					enemyList[i].position.Y = teleporter2location.powerlocation.Y-2;
					enemyList[i].position.X = teleporter2location.powerlocation.X;
					telecd = true;
				}
			}
		}
		//teleporter 2
		for(int i = 0; i<6; i++)
		{
			if(enemyList[i].position.Y == teleporter2location.powerlocation.Y - 2 && enemyList[i].position.X == teleporter2location.powerlocation.X)
			{
				if(telecd == false)
				{
					enemyList[i].position.Y = teleporter1location.powerlocation.Y-2;
					enemyList[i].position.X = teleporter1location.powerlocation.X;
					telecd = true;
				}
			}
		}
		telecd = false;
	}
}

void activatepowerups(activatepowerup& state,bool activated)
{
	afire.activated = false;
}

void setrandomlevelpos(randomlevelpos& random,int levelpos)
{
	//random fire level
	lfire.levelpos = 4;
}

void settimeStamps(timestamps& timing,double timestamp, bool stamptime)
{
	//teleporter
	tele.timestamp = 0.0;
	tele.stamptime = true;

	//life powerup
	life.timestamp = 0.0;
	life.stamptime = true;

	//fire powerup
	fire.timestamp = 0.0;
	life.stamptime = true;

	//flame lifetime
	flame.timestamp = 0.0;
	flame.stamptime = true;

	//freeze powerup
	freeze.timestamp = 0.0;
	freeze.stamptime = true;
}

void setpowerups(powerups& spawning,bool present)
{
	//life powerup
	plife.present = false;

	//fire powerup
	pfire.present = false;

	//freeze powerup
	pfreeze.present = false;
}

void extralifepowerup()
{
	bool getpowerup = true;//able to pick up power up
	int randomloop = rand() % 24 + 1;

	//change random lifepowerup location
	if(plife.present == false)
	{
		for(int n = 0;n < randomloop;n++)
		{
			if(randomloop - 1 == n)
			{
				lifepoweruplocation.powerlocation.X = (n * 3) + 1;
			}
		}
		lifepoweruplocation.powerlocation.Y = (3);
	}
	//lifepowerup random spawn time
	if(life.stamptime == true && plife.present == false)
	{
		life.timestamp = elapsedTime;
		life.stamptime = false;
	}
	if(elapsedTime >life.timestamp + rand() % 10 + 5 && plife.present == false)//change random spawn timing here
	{
		plife.present = true;
		getpowerup = true;
		life.stamptime = true;
	}

	//lifepowerup
	if(getpowerup == true && plife.present == true)
	{
		if(charLocation.X == lifepoweruplocation.powerlocation.X)
		{
			for(int i = 0; i<3; i++)
			{
				if(banana[i].active == false)
				{
					banana[i].active = true;
					break;
				}
			}
			plife.present = false;
			getpowerup = false;
		}
	}
}

void firepowerup()
{
	bool getfire = true;//able to pick up power up
	int randomloop = rand() % 24 + 1;
	int randomnum = rand() % 3 + 1;

	//change random firepowerup location
	if(pfire.present== false)
	{
		for(int n = 0;n < randomloop;n++)
		{
			if(randomloop - 1 == n)
			{
				firepoweruplocation.powerlocation.X = (n * 3) + 1;
			}
		}
		firepoweruplocation.powerlocation.Y = (3);
	}

	//firepowerup random spawn time
	if(fire.stamptime == true && pfire.present == false)
	{
		fire.timestamp = elapsedTime;
		fire.stamptime = false;
	}
	if(elapsedTime > fire.timestamp + rand() % 10 + 5 && pfire.present == false)//change random spawn timing here
	{
		pfire.present = true;
		getfire = true;
		fire.stamptime = true;
	}

	//fire power up effect
	if(getfire == true && pfire.present == true)
	{
		if(charLocation.X == firepoweruplocation.powerlocation.X)
		{
			flameslocation.powerlocation.X = 0;
			switch(randomnum)
			{
			case 3: flameslocation.powerlocation.Y = 15;
				break;
			case 2: flameslocation.powerlocation.Y = 21;
				break;
			case 1: flameslocation.powerlocation.Y = 27;
			}
			//kill enemies
			for(int i = 0; i<6; i++)
			{
				if(enemyList[i].position.Y == flameslocation.powerlocation.Y - 1)
				{
					enemyList[i].health = 0;
				}
			}
			pfire.present = false;
			getfire = false;
			afire.activated = true;
			flame.timestamp = elapsedTime;
		}
	}
}

void drawtele()
{
	gotoXY(teleporter1location.powerlocation);
	colour(0x0C);
	std::cout << (char)5;
	std::cout << (char)5;
	gotoXY(teleporter2location.powerlocation);
	colour(0x0C);
	std::cout << (char)5;
	std::cout << (char)5;
}

void drawlife()
{
	if(plife.present == true)
	{
		gotoXY(lifepoweruplocation.powerlocation);
		colour(0x0C);
		std::cout<<(char)3;
	}
}
void drawfire()
{
	if(pfire.present == true)
	{
		gotoXY(firepoweruplocation.powerlocation);
		colour(0x0C);
		std::cout<<(char)21;
	}
}

void drawflame()
{
	if(afire.activated == true)
	{
		gotoXY(flameslocation.powerlocation);
		colour(0x4E);//orange
		for(int n = 0; n < 79;n++)
		{
			std::cout<<(char)15;
		}
		std::cout<<std::endl;
		for(int n = 0; n < 79;n++)
		{
			std::cout<<(char)15;
		}
		if(elapsedTime > flame.timestamp + 1)//flame lifetime
		{
			afire.activated = false;
		}
	}
}