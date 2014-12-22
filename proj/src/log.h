#pragma once

#include "bitmap.h"

typedef struct{

	int x,y;
	double vx,vy;

	int width;

	int faixa;
} Log;

Log* newLog(int faixa);
void updateLog(Log * log);
void drawLog(Log * log, Bitmap * img);
void deleteLog(Log * log);
