#include "MainMenu.h"
#include "rectangle.h"
#include "mouse.h"
#include "path.h"
#include "graphics.h"
#include "colors.h"
#include "strings.h"

MainMenu* newMainMenu(){
	MainMenu* menu = (MainMenu *) malloc(sizeof(MainMenu));
	menu->done = 0;
	menu->fundo = loadBitmap(getPath("frogger.bmp"));
	menu->mousePlay = 0;
	menu->playButton = newRec(0.32*get_h_res(),0.27*get_h_res(),0.68*get_h_res(),0.315*get_h_res());
	menu->mouseHighScores = 0;
	menu->highScoresButton = newRec(0.32*get_h_res(),0.3575*get_h_res(),0.68*get_h_res(),0.4025*get_h_res());
	menu->mouseSettings = 0;
	menu->settingsButton = newRec(0.32*get_h_res(),0.445*get_h_res(),0.68*get_h_res(),0.49*get_h_res());
	menu->mouseExit = 0;
	menu->exitButton = newRec(0.32*get_h_res(),0.5325*get_h_res(),0.68*get_h_res(),0.5775*get_h_res());
	return menu;
}

void updateMainMenu(MainMenu* menu, unsigned long scanCode){
	Mouse * m = getMouse();
	if(m->leftButtonDown){
		if(mouseInsideRec(menu->playButton)){
			menu->mousePlay = 1;
			menu->done = 1;
		}
		else if(mouseInsideRec(menu->highScoresButton)){
			menu->mouseHighScores = 1;
			menu->done = 1;
		}
		else if(mouseInsideRec(menu->settingsButton)){
			menu->mouseSettings = 1;
			menu->done = 1;
		}
		else if(mouseInsideRec(menu->exitButton)){
			menu->mouseExit = 1;
			menu->done = 1;
		}
	}
}

void drawMainMenu(MainMenu* menu){
	drawBitmap(menu->fundo,0,0,ALIGN_LEFT);
	if(mouseInsideRec(menu->playButton)){
		drawString("play", 0.45*get_h_res(), 0.27 *get_h_res(), RGB888toRGB565(255,255,0));
	}
	else{
		drawString("play", 0.455*get_h_res(), 0.275 *get_h_res(), RGB888toRGB565(255,255,0));
	}
	if(mouseInsideRec(menu->highScoresButton)){
		drawString("highscores", 0.40*get_h_res(), 0.3575*get_h_res(), RGB888toRGB565(255,255,0));
	}
	else{
		drawString("highscores", 0.405*get_h_res(), 0.3625 *get_h_res(), RGB888toRGB565(255,255,0));
	}
	if(mouseInsideRec(menu->settingsButton)){
		drawString("settings", 0.425*get_h_res(), 0.445*get_h_res(), RGB888toRGB565(255,255,0));
	}
	else{
		drawString("settings", 0.43*get_h_res(), 0.45 *get_h_res(), RGB888toRGB565(255,255,0));
	}
	if(mouseInsideRec(menu->exitButton)){
		drawString("exit", 0.45*get_h_res(), 0.5325*get_h_res(), RGB888toRGB565(255,255,0));
	}
	else{
		drawString("exit", 0.455*get_h_res(), 0.5375 *get_h_res(), RGB888toRGB565(255,255,0));
	}

}
void deleteMainMenu(MainMenu* menu){
	deleteRec(menu->playButton);
	deleteRec(menu->highScoresButton);
	deleteRec(menu->exitButton);
	deleteBitmap(menu->fundo);
	free(menu);
}
