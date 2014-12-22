#include "car.h"
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Car* newCar(int faixa){
	Car* car = (Car*)malloc(sizeof(Car));
	car->faixa = faixa;
	car->vy = 0;
	car->x = rand() % 561 + 120;
	switch(faixa){
	case 1:
		car->y = 0.65*get_h_res();
		car->vx = -(40/getFPS());
		break;
	case 2:
		car->y = 0.6*get_h_res();
		car->vx = 40/getFPS();
		break;
	case 3:
		car->y = 0.55*get_h_res();
		car->vx = -(40/getFPS());
		break;
	case 4:
		car->y = 0.50*get_h_res();
		car->vx = 40/getFPS();
		break;
	default:
		break;
	}
	return car;
}
void updateCar(Car * car){
	car->x = car->x + car->vx;
	if(car->x >= 0.85*get_h_res()){
		car->x = 0.15*get_h_res();
	}
	if(car->x <= 0.1*get_h_res()){
		car->x = 0.8*get_h_res();
	}
}
void drawCar(Car * car, Bitmap * img){
	int width = img->bitmapInfoHeader.width;
	int drawWidth = width/4;
	int height = img->bitmapInfoHeader.height;

	if (car->x + drawWidth < 0.15*get_h_res() || car->x > 0.8*get_h_res() || car->y + height < 0
			|| car->y > get_v_res())
		return;


	char* bufferStartPos;
	char* imgStartPos;

	int i,j;
	for (i = 0; i < height; i++) {
		int pos = car->y + height - 1 - i;

		if (pos < 0 || pos >= get_v_res())
			continue;

		bufferStartPos = (char*)getBuffer();
		bufferStartPos += car->x * 2 + pos * get_h_res() * 2;

		imgStartPos = img->bitmapData + i * width * 2;
		switch (car->faixa){
		case 1:
			imgStartPos += (drawWidth * 2)*(car->faixa - 1);
			break;
		case 2:
			imgStartPos += (drawWidth * 2)*(car->faixa - 1);
			break;
		case 3:
			imgStartPos += (drawWidth * 2)*(car->faixa);
			break;
		case 4:
			imgStartPos += (drawWidth * 2)*(car->faixa - 2);
			break;
		default:
			break;
		}

		for(j=0; j < drawWidth; j++){
			if(j > 0.2*get_h_res() - car->x && j < 0.8*get_h_res() - car->x){
				if((*imgStartPos | ((*(imgStartPos+1))>>4)) != 0){
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
void deleteCar(Car * car){
	free(car);
}
