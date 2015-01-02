#include "turtles.h"
#include "bitmap.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int counter = 0;

Turtles* newTurtle(int faixa){
	Turtles* t = (Turtles*)malloc(sizeof(Turtles));
	t->faixa = faixa;
	t->vy = 0;
	t->x = rand() % 561 + 120;
	switch(faixa){
	case 1:
		t->width = 0.12375 * get_h_res();
		t->y = 0.35 *get_h_res();
		t->vx = -40/getFPS() * 2;
		break;
	case 4:
		t->width = 0.08125 * get_h_res();
		t->y = 0.20 *get_h_res();
		t->vx = -(40/getFPS())*2;
		break;
	default:
		break;
	}
	int num = rand() % 10;
	t->anim = 0;
	t->desenho = 0;
	if(num >= 8){
		t->anim = 1;
		t->desenho = rand() % 3;
	}
	return t;
}
void updateTurtle(Turtles * t){
	counter++;
	t->x = t->x + t->vx;
	if(t->x >= 0.85*get_h_res()){
		t->x = 0.2*get_h_res() - t->width;
	}
	if(t->x <= 0.15*get_h_res()- t->width){
		t->x = 0.8*get_h_res();
	}
	if(t->anim){
		switch(t->desenho){
		case 0:
			if(getTimer()->counter % 240 == 0){
				t->desenho++;
			}
			break;
		case 1:
			if(getTimer()->counter % 30 == 0){
				t->desenho++;
			}
			break;
		case 2:
			if(getTimer()->counter % 30 == 0){
				t->desenho++;
			}
			break;
		case 3:
			if(getTimer()->counter % 30 == 0){
				t->desenho = 0;
			}
			break;
		default:
			break;
		}
	}
}
void drawTurtle(Turtles * t, Bitmap * img){
	int width = img->bitmapInfoHeader.width;
	int drawWidth = width/4;
	int height = img->bitmapInfoHeader.height;

	if (t->x + drawWidth < 0.2*get_h_res() || t->x > 0.8*get_h_res() || t->y + height < 0 || t->y > get_v_res())
		return;


	char* bufferStartPos;
	char* imgStartPos;

	int i,j;
	for (i = 0; i < height; i++) {
		int pos = t->y + height - 1 - i;

		if (pos < 0 || pos >= get_v_res())
			continue;

		bufferStartPos = (char*)getBuffer();
		bufferStartPos += t->x * 2 + pos * get_h_res() * 2;

		imgStartPos = img->bitmapData + i * width * 2;

		int sum = t->width * 2 * t->desenho;
		imgStartPos += sum;

		for(j=0; j < drawWidth; j++){
			if(j > 0.2*get_h_res() - t->x && j < 0.8*get_h_res() - t->x){
				if((*imgStartPos) != 0 || (*(imgStartPos+1)) != 0){
					*bufferStartPos = *imgStartPos;
					*(bufferStartPos+1) = *(imgStartPos+1);
				}
			}
			bufferStartPos++;
			bufferStartPos++;
			imgStartPos++;
			imgStartPos++;
		}
	}
}
void deleteTurtle(Turtles * t){
	free(t);
}

