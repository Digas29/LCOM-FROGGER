#pragma once
#include "timer.h"
#include "bitmap.h"
#include "MainMenu.h"

typedef enum{
	MAIN_MENU,
	GAME,
	HIGHSCORES_MENU

} State;

typedef struct {
	int IRQ_KB;
	int IRQ_TIMER;
	int IRQ_M;

	int complete, refresh;

	State estado;
	void * state;

	unsigned long scanCode;
	Timer* timer;
} Frogger;


Frogger* newFrogger();
void updateFrogger(Frogger* frogger);
void drawFrogger(Frogger* frogger);
void deleteFrogger(Frogger* frogger);

