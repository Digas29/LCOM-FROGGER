#pragma once
#include "timer.h"

typedef struct {
	int IRQ_KB;
	int IRQ_TIMER;
	int IRQ_M;

	int complete, refresh;

	unsigned long scanCode;
	Timer* timer;
} Frogger;

Frogger* newFrogger();
void updateFrogger(Frogger* frogger);
void drawFrogger(Frogger* frogger);
void deleteFrogger(Frogger* frogger);

