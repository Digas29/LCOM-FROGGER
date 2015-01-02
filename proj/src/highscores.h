#pragma once

#include "bitmap.h"
#include "rectangle.h"

typedef struct{
	int points;
	char * nome;
	char * data;
}Record;

typedef struct{
	int size;
	Record* records[10];
}Records;

typedef struct{
	int done;

	Bitmap* fundo;

	int mouseExit;
	Rec* exitButton;

}HighScoresMenu;

void newRecords();
Records * getRecords();
void loadRecords();
void deleteRecords();

HighScoresMenu* newHighScoresMenu();
void updateHighScoresMenu(HighScoresMenu* menu, unsigned long scanCode);
void drawHighScoresMenu(HighScoresMenu* menu);
void deleteHighScoresMenu(HighScoresMenu* menu);
