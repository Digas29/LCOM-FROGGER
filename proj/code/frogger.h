#pragma once
#include "timer.h"
#include "bitmap.h"
#include "MainMenu.h"

typedef enum{
	MAIN_MENU,
	GAME,
	SETTINGS,
	HIGHSCORES_MENU,
	NEWRECORD

} State;

typedef struct {
	int IRQ_KB;
	int IRQ_TIMER;
	int IRQ_M;
	int IRQ_R;

	int complete, refresh;

	State estado;
	void * state;
	int draw;
	int up;

	unsigned long scanCode;
} Frogger;


Frogger* newFrogger();
void updateFrogger(Frogger* frogger);
void drawFrogger(Frogger* frogger);
void deleteFrogger(Frogger* frogger);

void changeState(Frogger* frogger, State newSate);
void updateState(Frogger* frogger);
void deleteState(Frogger* frogger);

