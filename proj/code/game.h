#pragma once

#include "bitmap.h"
#include "car.h"
#include "log.h"
#include "turtles.h"

#define laneMaxSize 7
#define riverMaxSize 5
#define tMaxSize 5

typedef struct{

	int x,y;
	int newX,newY;
	double vx,vy;

	int swamp;

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
	Turtles* t[tMaxSize];
	int size;
} RiverT;


typedef struct{
	Bitmap * fundo;
	Bitmap * camiao;
	Bitmap * carros;
	Bitmap * troncos;
	Bitmap * tartarugas2;
	Bitmap * tartarugas3;
	Bitmap * toca;
	Bitmap * vidas;

	int sapos[5];

	int pontos;
	int pause;
	int level;
	int alarm;
	int gameover;
	int lives;
	int done;
	int newRecord;

	Frog* frog;

	Lane* lane1;
	Lane* lane2;
	Lane* lane3;
	Lane* lane4;
	Lane* lane5;

	RiverT* river1;
	River* river2;
	River* river3;
	RiverT* river4;
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

void levelupdate(Game* game);

int checkLaneCollisions(Frog * frog, Lane* lane);
int checkTruckCollisions(Frog * frog, Lane* lane);
int checkRiverCollisions(Frog * frog, River* river);
int checkRiverTCollisions(Frog * frog, RiverT* river);
int checkSwampCollision(Frog * frog);

void addCar(Lane* lane, int faixa);
void addTruck(Lane* lane, int faixa);
void addLog(River* river, int faixa);
void addTurtles(RiverT* river, int faixa);
