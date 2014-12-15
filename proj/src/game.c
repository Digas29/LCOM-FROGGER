#include "game.h"
#include "frogger.h"
#include "keyboard.h"
#include "car.h"

Game* newGame(){
	Game * jogo = (Game*) malloc(sizeof(Game));

	jogo->fundo = loadBitmap("/home/proj/res/test.bmp");
	jogo->carros = loadBitmap("/home/proj/res/car_sprites.bmp");

	jogo->frog = newFrog();

	jogo->lane1 = (Lane*)malloc(sizeof(Lane));
	jogo->lane2 = (Lane*)malloc(sizeof(Lane));
	jogo->lane3 = (Lane*)malloc(sizeof(Lane));
	jogo->lane4 = (Lane*)malloc(sizeof(Lane));

	jogo->lane1->cars[0] = newCar(1);
	jogo->lane1->size = 1;
	jogo->lane2->cars[0] = newCar(2);
	jogo->lane2->size = 1;
	jogo->lane3->cars[0] = newCar(3);
	jogo->lane3->size = 1;
	jogo->lane4->cars[0] = newCar(4);
	jogo->lane4->size = 1;

	jogo->lives = 3;

	jogo->gameover = 0;
	jogo->done = 0;
	return jogo;
}
void updateGame(Game* game, unsigned long scanCode){
	if(scanCode == KEY_ESC){
		game->done = 1;
	}
	updateFrog(game->frog, scanCode);
	if(!game->frog->dead){
		int i;
		for(i = 0; i < game->lane1->size; i++){
			updateCar(game->lane1->cars[i]);
		}
		for(i = 0; i < game->lane2->size; i++){
			updateCar(game->lane2->cars[i]);
		}
		for(i = 0; i < game->lane3->size; i++){
			updateCar(game->lane3->cars[i]);
		}
		for(i = 0; i < game->lane4->size; i++){
			updateCar(game->lane4->cars[i]);
		}

		int collision = 0;

		switch(game->frog->faixa){
		case 1:
			collision = checkLaneCollisions(game->frog, game->lane1);
			break;
		case 2:
			collision = checkLaneCollisions(game->frog, game->lane2);
			break;
		case 3:
			collision = checkLaneCollisions(game->frog, game->lane3);
			break;
		case 4:
			collision = checkLaneCollisions(game->frog, game->lane4);
			break;
		default:
			break;
		}
		if(collision){
			game->lives--;
			game->frog->dead = 1;
			if(!game->lives){
				game->done = 1;
			}
			game->frog->dir = 5;
		}
	}
}
void drawGame(Game* game){
	drawBitmap(game->fundo, 0, 0, ALIGN_LEFT);
	drawFrog(game->frog);
	int i;
	for(i = 0; i < game->lane1->size; i++){
		drawCar(game->lane1->cars[i], game->carros);
	}
	for(i = 0; i < game->lane2->size; i++){
		drawCar(game->lane2->cars[i], game->carros);
	}
	for(i = 0; i < game->lane3->size; i++){
		drawCar(game->lane3->cars[i], game->carros);
	}
	for(i = 0; i < game->lane4->size; i++){
		drawCar(game->lane4->cars[i], game->carros);
	}
}
void deleteGame(Game* game){
	free(game);
}

Frog* newFrog(){
	Frog * sapo = (Frog*) malloc(sizeof(Frog));

	sapo->img = loadBitmap("/home/proj/res/frog_sprites.bmp");
	sapo->dir = 1;
	sapo->anim = 0;

	sapo->x = 380;
	sapo->y = 560;
	sapo->vx = 0;
	sapo->vy = 0;

	sapo->newX = 0;
	sapo->newY = 0;

	sapo->faixa = 0;
	sapo->dead = 0;

	return sapo;
}
void updateFrog(Frog* sapo,unsigned long scanCode){
	if(sapo->dead){
		switch(sapo->dir){
		case 5:
			sapo->dir = 6;
			return;
		case 6:
			sapo->dir = 7;
			return;
		case 7:
			sapo->faixa = 0;
			sapo->x = 380;
			sapo->dir = 1;
			sapo->y = 560;
			sapo->dead = 0;
			sapo->vy = 0;
			sapo->vx = 0;
			return;
		default:
			break;
		}
	}
	if(sapo->vx == 0 && sapo->vy == 0){
		switch(scanCode){
		case KEY_W:
			sapo->dir = 1;
			if(sapo->y - 40 < 0){
				sapo->newY = 0;
			}
			else{
				sapo->anim = 1;
				sapo->newY = sapo->y - 40;
				sapo->vy = - (40/getFPS());
				sapo->faixa++;
			}
			break;
		case KEY_S:
			sapo->dir = 3;
			if(sapo->y + 40 > 560){
				sapo->newY = 560;
			}
			else{
				sapo->anim = 1;
				sapo->newY = sapo->y + 40;
				sapo->vy = (40/getFPS());
				sapo->faixa--;
			}
			break;
		case KEY_D:
			sapo->dir = 2;
			if(sapo->x + 40 > 600){
				sapo->newY = 600;
			}
			else{
				sapo->anim = 1;
				sapo->newX = sapo->x + 40;
				sapo->vx = (40/getFPS());
			}
			break;
		case KEY_A:
			sapo->dir = 4;
			if(sapo->x - 40 < 160){
				sapo->newY = 160;
			}
			else{
				sapo->anim = 1;
				sapo->newX = sapo->x - 40;
				sapo->vx = -(40/getFPS());
			}
			break;
		default:
			break;
		}
	}
	else{
		if(sapo->vy < 0){
			if(sapo->y + sapo->vy > sapo->newY){
				sapo->y = sapo->y + sapo->vy;
			}
			else{
				sapo->y = sapo->newY;
				sapo->vy = 0;
				sapo->anim = 0;
			}
		}
		else if (sapo->vy > 0){
			if(sapo->y + sapo->vy < sapo->newY){
				sapo->y = sapo->y + sapo->vy;
			}
			else{
				sapo->y = sapo->newY;
				sapo->vy = 0;
				sapo->anim = 0;
			}
		}
		if(sapo->vx < 0){
			if(sapo->x + sapo->vx > sapo->newX){
				sapo->x = sapo->x + sapo->vx;
			}
			else{
				sapo->x = sapo->newX;
				sapo->vx = 0;
				sapo->anim = 0;
			}
		}
		else if (sapo->vx > 0){
			if(sapo->x + sapo->vx < sapo->newX){
				sapo->x = sapo->x + sapo->vx;
			}
			else{
				sapo->x = sapo->newX;
				sapo->vx = 0;
				sapo->anim = 0;
			}
		}
	}
}

void drawFrog(Frog* sapo){

	int width = sapo->img->bitmapInfoHeader.width;
	int drawWidth = width/ 11;
	int height = sapo->img->bitmapInfoHeader.height;

	if (sapo->x + drawWidth < 160 || sapo->x > get_h_res() || sapo->y + height < 0
			|| sapo->y > get_v_res())
		return;


	char* bufferStartPos;
	char* imgStartPos;

	int i,j;
	for (i = 0; i < height; i++) {
		int pos = sapo->y + height - 1 - i;

		if (pos < 0 || pos >= get_v_res())
			continue;

		bufferStartPos = (char*)getBuffer();
		bufferStartPos += sapo->x * 2 + pos * get_h_res() * 2;

		imgStartPos = sapo->img->bitmapData + i * width * 2;

		switch(sapo->dir){
		case 1:
			break;
		case 2:
			imgStartPos += (drawWidth * 2) * 2;
			break;
		case 3:
			imgStartPos += (drawWidth * 2) * 4;
			break;
		case 4:
			imgStartPos += (drawWidth * 2) * 6;
			break;
		case 5:
			imgStartPos += (drawWidth * 2) * 8;
			break;
		case 6:
			imgStartPos += (drawWidth * 2) * 9;
			break;
		case 7:
			imgStartPos += (drawWidth * 2) * 10;
			break;
		default:
			break;
		}
		if(sapo->anim){
			imgStartPos += (drawWidth * 2);
		}

		for(j=0; j < drawWidth; j++){
			if((*imgStartPos | ((*(imgStartPos+1))>>4)) != 0){
				*bufferStartPos = *imgStartPos;
				*(bufferStartPos+1) = *(imgStartPos+1);
			}
			bufferStartPos++;
			bufferStartPos++;
			imgStartPos++;
			imgStartPos++;
		}
	}
}
void deleteFrog(Frog* sapo){
	free(sapo);
}

int checkLaneCollisions(Frog * frog, Lane* lane){
	int i;
	for(i = 0;i < lane->size; i++){
		if(!(frog->x + 26 < lane->cars[i]->x || frog->x > lane->cars[i]->x + 26 ||
				frog->y + 26 < lane->cars[i]->y || frog->y > lane->cars[i]->y + 26))
			return 1;
	}
	return 0;
}