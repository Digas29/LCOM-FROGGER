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
void addRecord(Record* record){
	int pos = records->size;
	if(pos == 10){
		pos--;
	}
	records->records[pos] = record;
	records->size++;
	Record * temp;
	int i,j;
	for (i = 0; i < records->size; ++i)
	{
		for (j = i + 1; j < records->size; ++j)
		{
			if (records->records[i]->points < records->records[j]->points)
			{
				temp =  records->records[i];
				records->records[i] = records->records[j];
				 records->records[j] = temp;
			}
		}
	}
}
Records * getRecords(){
	return records;
}
void deleteRecords(){
	free(records);
}
void loadRecords(){
	int letra;
	char * name;
	char * date;
	int points;
	char * path = getHighPath("highscores.txt");
	FILE *file;

	file = fopen(path,"r");

	if (!file)
		return;
	while (fgetc(file) != EOF){
		letra  = fgetc(file);
		printf("%c \n", letra);
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
	int i;
	int x = 0.2*get_h_res();
	int y = 0.2*get_h_res();
	for(i=0; i < records->size; i++){
		drawString(records->records[i]->data, x,y, RGB888toRGB565(255,255,255));
		drawString(records->records[i]->nome, x + 0.20 *get_h_res(),y, RGB888toRGB565(255,255,255));
		char point[15];
		sprintf(point, "%d", records->records[i]->points);
		drawString(point, x + 2*0.2 *get_h_res(),y, RGB888toRGB565(255,255,255));
		y += 0.05*get_h_res();
	}
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
