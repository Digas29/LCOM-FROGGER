#include "car.h"
#include "bitmap.h"

Car* newCar(int faixa){
	Car* car = (Car*)malloc(sizeof(Car));
	car->faixa = faixa;
	car->vy = 0;
	switch(faixa){
	case 1:
		car->x = 640;
		car->y = 520;
		car->vx = -(40/getFPS());
		break;
	case 2:
		car->x = 120;
		car->y = 480;
		car->vx = 40/getFPS();
		break;
	case 3:
		car->x = 640;
		car->y = 440;
		car->vx = -(40/getFPS());
		break;
	case 4:
		car->x = 120;
		car->y = 400;
		car->vx = 40/getFPS();
		break;
	default:
		break;
	}
	return car;
}
void updateCar(Car * car){
	car->x = car->x + car->vx;
}
void drawCar(Car * car, Bitmap * img){
	int width = img->bitmapInfoHeader.width;
	int drawWidth = width/4;
	int height = img->bitmapInfoHeader.height;

	if (car->x + drawWidth < 120 || car->x > 640 || car->y + height < 0
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
			if(j > 160 - car->x && j < 640 - car->x){
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
