#include"teleporters.h"

extern double elapsedTime;
extern COORD charLocation;
extern COORD playerhumanLocation;
extern int killCount;


powercoord teleporter1location, teleporter2location, lifepoweruplocation, firepoweruplocation, flameslocation, freezepoweruplocation;
powerups life, fire, freeze, tele, flames, frozen;

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
	if(elapsedTime>5)//time before teleporters spawn
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

void setpowerups(powerups& spawning,bool present)
{
	//teleporter
	tele.timestamp = 0.0;
	tele.stamptime = true;

	//life powerup
	life.present = false;
	life.timestamp = 0.0;
	life.stamptime = true;

	//fire powerup
	fire.present = false;
	fire.activated = false;
	fire.timestamp = 0.0;
	fire.stamptime = true;

	//flames
	flames.timestamp = 0.0;
	flames.stamptime = true;

	//freeze powerup
	freeze.present = false;
	freeze.activated = false;
	freeze.timestamp = 0.0;
	freeze.stamptime = true;

	//frozen
	frozen.timestamp = 0.0;
	frozen.stamptime = true;
}

void resetpowerup()
{
	//teleporter
	tele.timestamp = 0.0;
	tele.stamptime = true;

	//life powerup
	life.present = false;
	life.timestamp = 0.0;
	life.stamptime = true;

	//fire powerup
	fire.present = false;
	fire.activated = false;
	fire.timestamp = 0.0;
	fire.stamptime = true;

	//flames
	flames.timestamp = 0.0;
	flames.stamptime = true;

	//freeze powerup
	freeze.present = false;
	freeze.activated = false;
	freeze.timestamp = 0.0;
	freeze.stamptime = true;

	//frozen
	frozen.timestamp = 0.0;
	frozen.stamptime = true;
}

void extralifepowerup()
{
	bool getpowerup = true;//able tp pick up power up
	int randomloop = rand() % 24 + 1;

	//change random lifepowerup location
	if(life.present == false)
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
	if(life.stamptime == true && life.present == false)
	{
		life.timestamp = elapsedTime;
		life.stamptime = false;
	}
	if(elapsedTime > life.timestamp + rand() % 10 + 5 && life.present == false)//change random spawn timing here
	{
		life.present = true;
		getpowerup = true;
		life.stamptime = true;
	}

	//lifepowerup
	if(getpowerup == true && life.present == true)
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
			life.present = false;
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
	if(fire.present== false)
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
	if(fire.stamptime == true && fire.present == false)
	{
		fire.timestamp = elapsedTime;
		fire.stamptime = false;
	}
	if(elapsedTime > fire.timestamp + rand() % 10 + 5 && fire.present == false)//change random spawn timing here
	{
		fire.present = true;
		getfire = true;
		fire.stamptime = true;
	}

	//fire power up effect
	if(getfire == true && fire.present == true)
	{
		if(charLocation.X == firepoweruplocation.powerlocation.X)
		{
			flameslocation.powerlocation.X = 0;
			switch(randomnum)
			{
			case 3: flameslocation.powerlocation.Y = 14;
				break;
			case 2: flameslocation.powerlocation.Y = 20;
				break;
			case 1: flameslocation.powerlocation.Y = 26;
			}
			fire.present = false;
			getfire = false;
			fire.activated = true;
			flames.timestamp = elapsedTime;
		}
	}

	//kill enemies
	if(fire.activated == true)
	{
		for(int i = 0; i<enemies; i++)
			{
				if(enemyList[i].position.Y == flameslocation.powerlocation.Y - 1)
				{
					enemyList[i].health = 0;
				}
			}
	}
}

void freezepowerup()
{
	bool getfreeze = true;//able to pick up powerup
	int randomloop = rand() % 24 + 1;

	//change random lifepowerup location
	if(freeze.present == false)
	{
		for(int n = 0;n < randomloop;n++)
		{
			if(randomloop - 1 == n)
			{
				freezepoweruplocation.powerlocation.X = (n * 3) + 1;//align position with player
			}
		}
		freezepoweruplocation.powerlocation.Y = (3);
	}

	//freezepowerup random spawn time
	if(freeze.stamptime == true && freeze.present == false)
	{
		freeze.timestamp = elapsedTime;
		freeze.stamptime = false;
	}
	if(elapsedTime > freeze.timestamp + rand() % 10 + 5 && freeze.present == false)//change random spawn timing here
	{
		freeze.present = true;
		getfreeze = true;
		freeze.stamptime = true;
	}

	//freeze powerup effect
	if(getfreeze == true && freeze.present == true)
	{
		if(charLocation.X == freezepoweruplocation.powerlocation.X)//pick up powerup
		{
			for(int i = 0; i<6; i++)
			{
				enemyList[i].canMove = false;
			}
			freeze.present = false;//change herer
			getfreeze = false;
			frozen.timestamp = elapsedTime;
		}
	}

	//unfreeze enemies
	if(elapsedTime > frozen.timestamp + 3)
	{
			for(int i = 0; i<6; i++)
			{
				enemyList[i].canMove = true;
			}
	}
}

void drawtele()
{
	/*gotoXY(teleporter1location.powerlocation);
	colour(0x0C);
	std::cout << (char)5;
	std::cout << (char)5;*/
	writeToBuffer(teleporter1location.powerlocation, (char)5, 0x0C);
	/*gotoXY(teleporter2location.powerlocation);
	colour(0x0C);
	std::cout << (char)5;
	std::cout << (char)5;*/
	writeToBuffer(teleporter2location.powerlocation, (char)5, 0x0C);
}

void drawlife()
{
	if(life.present == true)
	{
		/*gotoXY(lifepoweruplocation.powerlocation);
		colour(0x0C);
		std::cout<<(char)3;*/
		writeToBuffer(lifepoweruplocation.powerlocation, (char)3, 0x0C);
	}
}

void drawfire()
{
	if(fire.present == true)
	{
		/*gotoXY(firepoweruplocation.powerlocation);
		colour(0x0C);
		std::cout<<(char)21;*/
		writeToBuffer(firepoweruplocation.powerlocation, (char)21, 0x0C);
	}
}

void drawflame()
{
	if(fire.activated == true)
	{
		//gotoXY(flameslocation.powerlocation);
		//colour(0x4E);//orange
		for(int n = 0; n < 77;n++)
		{
			/*std::cout<<(char)15;*/
			writeToBuffer(flameslocation.powerlocation, (char)15, 0x4E);
		}
		std::cout<<std::endl;
		for(int n = 0; n < 77;n++)
		{
			/*std::cout<<(char)15;*/
			writeToBuffer(flameslocation.powerlocation, (char)15, 0x4E);
		}
		if(elapsedTime > flames.timestamp + 1)//flame lifetime
		{
			fire.activated = false;
		}
	}
}

void drawfreeze()
{
	//render freeze power up
	if(freeze.present == true)
	{
		/*gotoXY(freezepoweruplocation.powerlocation);
		colour(0x0C);
		std::cout<<(char)4;*/
		writeToBuffer(freezepoweruplocation.powerlocation, (char)4, 0x0C);
	}
}