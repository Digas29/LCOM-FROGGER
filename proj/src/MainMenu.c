#include "MainMenu.h"
#include "mouse.h"
#include "graphics.h"

MainMenu* newMainMenu(){
	MainMenu* menu = (MainMenu *) malloc(sizeof(MainMenu));

	menu->done = 0;
	menu->fundo = loadBitmap("/home/proj/res/cenas.bmp");
	menu->mousePlay = 0;
	menu->playButton = newRec(300,200,500,250);
	menu->mouseHighScores = 0;
	menu->highScoresButton = newRec(300,300,500,350);
	menu->mouseExit = 0;
	menu->exitButton = newRec(300,400,500,450);

	return menu;
}
int mouseInsideRec(Rec* rect){
	if(getMouse()->x >= rect->xi &&
			getMouse()->x <= rect->xf &&
			getMouse()->y >= rect->yi &&
			getMouse()->y <= rect->yf){
		return 1;
	}
	else
		return 0;
}
void updateMainMenu(MainMenu* menu, unsigned long scanCode){
	Mouse * m = getMouse();
	if(m->leftButtonReleased){
		if(mouseInsideRec(menu->playButton)){
			menu->mousePlay = 1;
			menu->done = 1;
		}
		else{
			menu->mousePlay = 0;
		}
		if(mouseInsideRec(menu->highScoresButton)){
			menu->mouseHighScores = 1;
			menu->done = 1;
		}
		else{
			menu->mouseHighScores = 0;
		}
		if(mouseInsideRec(menu->exitButton)){
			menu->mouseExit = 1;
			menu->done = 1;
		}
		else{
			menu->mouseExit = 0;
		}
	}
}
void drawMainMenu(MainMenu* menu){
	drawBitmap(menu->fundo,0,0,ALIGN_LEFT);
	bufffer_draw_rectangle(300,200,200,50,0xF800);
	bufffer_draw_rectangle(300,300,200,50,0xF800);
	bufffer_draw_rectangle(300,400,200,50,0xF800);
}
void deleteMainMenu(MainMenu* menu){
	deleteBitmap(menu->fundo);
	deleteRec(menu->playButton);
	deleteRec(menu->highScoresButton);
	deleteRec(menu->exitButton);
	free(menu);
}
