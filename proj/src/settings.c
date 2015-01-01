#include "settings.h"
#include "bitmap.h"
#include "rectangle.h"
#include "colors.h"
#include "strings.h"
#include "graphics.h"
#include "keyboard.h"
#include "path.h"
#include "mouse.h"


SettingsMenu* newSettingsMenu(){
	SettingsMenu* menu = (SettingsMenu*) malloc(sizeof(SettingsMenu));

	menu->done = 0;
	menu->fundo = loadBitmap(getPath("settings.bmp"));
	menu->mode114 = 0;
	menu->mode114Button = newRec(0.32*get_h_res(),0.3575*get_h_res(),0.68*get_h_res(),0.4025*get_h_res());
	menu->mode117 = 0;
	menu->mode117Button = newRec(0.32*get_h_res(),0.445*get_h_res(),0.68*get_h_res(),0.49*get_h_res());
	menu->mouseExit = 0;
	menu->exitButton = newRec(0.32*get_h_res(),0.5325*get_h_res(),0.68*get_h_res(),0.5775*get_h_res());

	return menu;
}

void updateSettingsMenu(SettingsMenu* menu, unsigned long scanCode){
	if(scanCode == KEY_ESC){
		menu->done = 1;
	}
	Mouse * m = getMouse();
	if(m->leftButtonDown){
		if(mouseInsideRec(menu->mode114Button)){
			menu->mode114 = 1;
			menu->done = 1;
		}
		else if(mouseInsideRec(menu->mode117Button)){
			menu->mode117 = 1;
			menu->done = 1;
		}
		else if(mouseInsideRec(menu->exitButton)){
			menu->mouseExit = 1;
			menu->done = 1;
		}
	}
}

void drawSettingsMenu(SettingsMenu* menu){
	drawBitmap(menu->fundo, 0, 0,ALIGN_LEFT);
	if(mouseInsideRec(menu->mode114Button)){
		drawString("800x600", 0.425*get_h_res(), 0.3575*get_h_res(), RGB888toRGB565(255,255,0));
	}
	else{
	 	drawString("800x600", 0.43*get_h_res(), 0.3625 *get_h_res(), RGB888toRGB565(255,255,0));
	}
	if(mouseInsideRec(menu->mode117Button)){
		drawString("1024x768", 0.40*get_h_res(), 0.445*get_h_res(), RGB888toRGB565(255,255,0));
	}
	else{
		drawString("1024x768", 0.405*get_h_res(), 0.45 *get_h_res(), RGB888toRGB565(255,255,0));
	}
	if(mouseInsideRec(menu->exitButton)){
		drawString("back", 0.45*get_h_res(), 0.5325*get_h_res(), RGB888toRGB565(255,255,0));
	}
	else{
		drawString("back", 0.455*get_h_res(), 0.5375 *get_h_res(), RGB888toRGB565(255,255,0));
	}
}
void deleteSettingsMenu(SettingsMenu* menu){
	deleteRec(menu->mode114Button);
	deleteRec(menu->mode117Button);
	deleteBitmap(menu->fundo);
	free(menu);
}
