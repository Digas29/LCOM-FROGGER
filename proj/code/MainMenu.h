#pragma once

#include "bitmap.h"
#include "rectangle.h"


typedef struct{
	int done;

	Bitmap* fundo;

	int mousePlay;
	Rec * playButton;
	int mouseHighScores;
	Rec * highScoresButton;
	int mouseSettings;
	Rec* settingsButton;
	int mouseExit;
	Rec* exitButton;
}MainMenu;

MainMenu* newMainMenu();
void updateMainMenu(MainMenu* menu, unsigned long scanCode);
void drawMainMenu(MainMenu* menu);
void deleteMainMenu(MainMenu* menu);

