#pragma once
#ifndef _LEVEL_H
#define _LEVEL_H
#define WIDTH 80
#define HEIGHT 30
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "Framework\console.h"


using std::string;
using std::cout;
using std::endl;

void save (void);
void read (void);
void LoadMap(int a);
void DrawMap(char);

#endif // _LEVEL_H