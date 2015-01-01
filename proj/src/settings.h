#pragma once

#include "bitmap.h"
#include "rectangle.h"


typedef struct{
	int done;

	Bitmap* fundo;

	int mode114;
	Rec * mode114Button;
	int mode117;
	Rec* mode117Button;
	int mouseExit;
	Rec* exitButton;

}SettingsMenu;

SettingsMenu* newSettingsMenu();
void updateSettingsMenu(SettingsMenu* menu, unsigned long scanCode);
void drawSettingsMenu(SettingsMenu* menu);
void deleteSettingsMenu(SettingsMenu* menu);
