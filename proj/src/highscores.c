#include "highscores.h"
#include "path.h"
#include "mouse.h"
#include "bitmap.h"
#include "keyboard.h"
#include "rectangle.h"
#include "strings.h"
#include "colors.h"
#include <stdlib.h>
#include <stdio.h>

Records * records = NULL;


void newRecords(){
	records = (Records*)malloc(sizeof(Records));
	records->size = 0;
}
Records * getRecords(){
	return records;
}
void deleteRecords(){
	free(records);
}
void loadRecords(){
	char line[1024];
	char * name;
	char * date;
	int points;
	char * path = getHighPath("highscores.txt");
	FILE *file;

	file = fopen(path,"r");

	if (!file)
		return;
	while (fgets(line,1024, file)){
		name = &line[0];
		fgets(line,1024, file);
		date = &line[0];

		fscanf (file, "%d", &points);
		fgets(line,1024, file);
		printf("%s", name);
		printf("%s", date);
		printf("%d \n", points);
	}
	fclose(file);
}

HighScoresMenu* newHighScoresMenu(){
	HighScoresMenu* menu = (HighScoresMenu*)malloc(sizeof(HighScoresMenu));

	menu->done = 0;
	menu->fundo = loadBitmap(getPath("highscores.bmp"));

	menu->mouseExit = 0;
	menu->exitButton = newRec(0.32*get_h_res(),0.5325*get_h_res(),0.68*get_h_res(),0.5775*get_h_res());
	return menu;
}

void updateHighScoresMenu(HighScoresMenu* menu, unsigned long scanCode){
	if(scanCode == KEY_ESC){
		menu->mouseExit = 1;
		menu->done = 1;
	}
	Mouse * m = getMouse();
	if(m->leftButtonDown){
		if(mouseInsideRec(menu->exitButton)){
			menu->mouseExit = 1;
			menu->done = 1;
		}
	}
}

void drawHighScoresMenu(HighScoresMenu* menu){
	drawBitmap(menu->fundo, 0, 0,ALIGN_LEFT);

	if(mouseInsideRec(menu->exitButton)){
		drawString("back", 0.45*get_h_res(), 0.5325*get_h_res(), RGB888toRGB565(255,255,0));
	}
	else{
		drawString("back", 0.455*get_h_res(), 0.5375 *get_h_res(), RGB888toRGB565(255,255,0));
	}
}
void deleteHighScoresMenu(HighScoresMenu* menu){

	deleteRec(menu->exitButton);
	deleteBitmap(menu->fundo);
	free(menu);
}
