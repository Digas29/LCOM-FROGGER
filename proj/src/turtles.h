#pragma once

#include "bitmap.h"

typedef struct{

	int x,y;
	double vx,vy;
	int anim;
	int desenho;

	int width;

	int faixa;
} Turtles;

Turtles* newTurtle(int faixa);
void updateTurtle(Turtles * t);
void drawTurtle(Turtles * t, Bitmap * img);
void deleteTurtle(Turtles * t);
