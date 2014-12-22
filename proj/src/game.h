#pragma once

#include "bitmap.h"
#include "car.h"
#include "log.h"

#define laneMaxSize 7
#define riverMaxSize 5


typedef struct{

	int x,y;
	int newX,newY;
	double vx,vy;

	int dir;
	int anim;

	int dead;

	int faixa;

	Bitmap * img;
} Frog;

typedef struct{
	Car* cars[laneMaxSize];
	int size;
} Lane;

typedef struct{
	Log* logs[riverMaxSize];
	int size;
} River;


typedef struct{
	Bitmap * fundo;
	Bitmap * carros;
	Bitmap * troncos;

	int gameover;
	int lives;
	int done;
	Frog* frog;

	Lane* lane1;
	Lane* lane2;
	Lane* lane3;
	Lane* lane4;

	River* river2;
	River* river3;
	River* river5;
} Game;

Game* newGame();
void updateGame(Game* game, unsigned long scanCode);
void drawGame(Game* game);
void deleteGame(Game* game);

Frog* newFrog();
void updateFrog(Frog* sapo,unsigned long scanCode);
void drawFrog(Frog* sapo);
void deleteFrog(Frog* sapo);

int checkLaneCollisions(Frog * frog, Lane* lane);
