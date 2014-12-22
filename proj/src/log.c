#include "log.h"
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Log* newLog(int faixa){
	Log* log = (Log*)malloc(sizeof(Log));
	log->faixa = faixa;
	log->vy = 0;
	log->x = rand() % 561 + 120;
	switch(faixa){
	case 2:
		log->width = 0.11875 *get_h_res();
		log->y = 0.3 *get_h_res();
		log->vx = 40/getFPS() * 2;
		break;
	case 3:
		log->width = 0.245 *get_h_res();
		log->y = 0.25 *get_h_res();
		log->vx = -(40/getFPS())*2;
		break;
	case 5:
		log->width = 0.15875 *get_h_res();
		log->y = 0.15 *get_h_res();
		log->vx = 40/getFPS() * 2;
		break;
	default:
		break;
	}
	return log;
}
void updateLog(Log * log){
	log->x = log->x + log->vx;
	if(log->x >= 0.85*get_h_res()){
		log->x = 0.2*get_h_res()-log->width;
	}
	if(log->x <= 0.15*get_h_res()-log->width){
		log->x = 0.8*get_h_res();
	}
}
void drawLog(Log * log, Bitmap * img){
	int width = img->bitmapInfoHeader.width;
	int drawWidth = log->width;
	int height = img->bitmapInfoHeader.height;

	if (log->x + drawWidth < 0.2*get_h_res() || log->x > 0.8*get_h_res() || log->y + height < 0 || log->y > get_v_res())
		return;


	char* bufferStartPos;
	char* imgStartPos;

	int i,j;
	for (i = 0; i < height; i++) {
		int pos = log->y + height - 1 - i;

		if (pos < 0 || pos >= get_v_res())
			continue;

		bufferStartPos = (char*)getBuffer();
		bufferStartPos += log->x * 2 + pos * get_h_res() * 2;

		imgStartPos = img->bitmapData + i * width * 2;

		int sum;
		switch(log->faixa){
		case 3:
			sum = 0.11875 *get_h_res()*2;
			imgStartPos += sum;
			break;
		case 5:
			sum =  0.11875 *get_h_res()*2 + 0.245 *get_h_res()*2;
			imgStartPos += sum;
			break;
		default:
			break;

		}

		for(j=0; j < drawWidth; j++){
			if(j > 0.2*get_h_res() - log->x && j < 0.8*get_h_res() - log->x){
				if((*imgStartPos) != 0 && (*(imgStartPos+1)) != 0){
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
void deleteLog(Log * log){
	free(log);
}
