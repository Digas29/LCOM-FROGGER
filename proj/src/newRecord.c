#include "newRecord.h"
#include "bitmap.h"
#include "colors.h"
#include "strings.h"
#include "RTC.h"
#include "path.h"
#include "keyboard.h"
#include "highscores.h"
#include "strings.h"

newRecordMenu* newNewRecordMenu(int points){
	newRecordMenu* menu = (newRecordMenu*)malloc(sizeof(newRecordMenu));
	menu->done = 0;
	menu->points = points;

	unsigned long day,month,year;
	getDate(&day,&month,&year);
	year+=2000;
	char dayS[15];
	char monthS[15];
	char yearS[15];
	sprintf(dayS, "%lu", day);
	sprintf(monthS, "%lu", month);
	sprintf(yearS, "%lu", year);

	unsigned int len1 = strlen(dayS);
	unsigned int len2 = strlen(monthS);
	unsigned int len3 = strlen(yearS);

	char *result = (char*)malloc(len1+len2+len3+3);
	memcpy(result, dayS, len1);
	memcpy(result+len1, " ", 1);
	memcpy(result+len1+1, monthS, len2);
	memcpy(result+len1+len2+1, " ", 1);
	memcpy(result+len1+len2+2, yearS, len3+1);//+1 to copy the null-terminator
	menu->data = result;

	menu->nome = "";

	menu->fundo = loadBitmap(getPath("newHighscore.bmp"));
	return menu;
}
void updateNewRecordMenu(newRecordMenu* menu, unsigned long scanCode){
	if(strlen(menu->nome) == 5 && scanCode != KEY_BKSP && scanCode != KEY_ENTER){
		return;
	}
	Record * record = (Record*)malloc(sizeof(Record));
	char *result;
	switch(scanCode){
	case KEY_A:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "a", 2);
		menu->nome = result;
		break;
	case KEY_B:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "b", 2);
		menu->nome = result;
		break;
	case KEY_C:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "c", 2);
		menu->nome = result;
		break;
	case KEY_D:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "d", 2);
		menu->nome = result;
		break;
	case KEY_E:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "e", 2);
		menu->nome = result;
		break;
	case KEY_F:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "f", 2);
		menu->nome = result;
		break;
	case KEY_G:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "g", 2);
		menu->nome = result;
		break;
	case KEY_H:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "h", 2);
		menu->nome = result;
		break;
	case KEY_I:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "i", 2);
		menu->nome = result;
		break;
	case KEY_J:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "j", 2);
		menu->nome = result;
		break;
	case KEY_K:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "k", 2);
		menu->nome = result;
		break;
	case KEY_L:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "l", 2);
		menu->nome = result;
		break;
	case KEY_M:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "m", 2);
		menu->nome = result;
		break;
	case KEY_N:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "n", 2);
		menu->nome = result;
		break;
	case KEY_O:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "o", 2);
		menu->nome = result;
		break;
	case KEY_P:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "p", 2);
		menu->nome = result;
		break;
	case KEY_Q:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "q", 2);
		menu->nome = result;
		break;
	case KEY_R:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "r", 2);
		menu->nome = result;
		break;
	case KEY_S:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "s", 2);
		menu->nome = result;
		break;
	case KEY_T:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "t", 2);
		menu->nome = result;
		break;
	case KEY_U:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "u", 2);
		menu->nome = result;
		break;
	case KEY_V:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "v", 2);
		menu->nome = result;
		break;
	case KEY_W:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "w", 2);
		menu->nome = result;
		break;
	case KEY_X:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "x", 2);
		menu->nome = result;
		break;
	case KEY_Y:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "y", 2);
		menu->nome = result;
		break;
	case KEY_Z:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "z", 2);
		menu->nome = result;
		break;
	case KEY_SPACE:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), " ", 2);
		menu->nome = result;
		break;
	case KEY_0:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "0", 2);
		menu->nome = result;
		break;
	case KEY_1:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "1", 2);
		menu->nome = result;
		break;
	case KEY_2:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "2", 2);
		menu->nome = result;
		break;
	case KEY_3:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "3", 2);
		menu->nome = result;
		break;
	case KEY_4:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "4", 2);
		menu->nome = result;
		break;
	case KEY_5:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "5", 2);
		menu->nome = result;
		break;
	case KEY_6:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "6", 2);
		menu->nome = result;
		break;
	case KEY_7:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "7", 2);
		free(menu->nome);
		menu->nome = result;
		break;
	case KEY_8:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "8", 2);
		menu->nome = result;
		break;
	case KEY_9:
		result = (char*)malloc(strlen(menu->nome)+2);
		memcpy(result, menu->nome, strlen(menu->nome));
		memcpy(result+strlen(menu->nome), "9", 2);
		menu->nome = result;
		break;
	case KEY_BKSP:
		if(strlen(menu->nome) > 0){
			result = (char*)malloc(strlen(menu->nome));
			memcpy(result, menu->nome, strlen(menu->nome) - 1);
			memcpy(result+strlen(menu->nome)-1, "", 1);
			menu->nome = result;
		}
		break;
	case KEY_ENTER:
		record->nome = menu->nome;
		record->data = menu->data;
		record->points = menu->points;
		addRecord(record);
		menu->done = 1;
		break;
	default:
		break;
	}

}
void drawNewRecordMenu(newRecordMenu* menu){
	drawBitmap(menu->fundo, 0, 0,ALIGN_LEFT);
	drawString(menu->nome, 0.45*get_h_res(), 0.35*get_h_res(), RGB888toRGB565(255,255,255));
}
void deleteNewRecordMenu(newRecordMenu* menu){
	deleteBitmap(menu->fundo);
	free(menu);
}
