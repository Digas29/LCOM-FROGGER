#pragma once
#include "bitmap.h"

typedef struct{
	int done;
	int points;
	char * data;
	char * nome;

	Bitmap* fundo;

}newRecordMenu;

newRecordMenu* newNewRecordMenu(int points);
void updateNewRecordMenu(newRecordMenu* menu, unsigned long scanCode);
void drawNewRecordMenu(newRecordMenu* menu);
void deleteNewRecordMenu(newRecordMenu* menu);
