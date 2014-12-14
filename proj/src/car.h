#pragma once

#include "bitmap.h"

typedef struct{

	int x,y;
	double vx,vy;

	int faixa;
} Car;

Car* newCar(int faixa);
void updateCar(Car * car);
void drawCar(Car * car, Bitmap * img);
void deleteCar(Car * car);
