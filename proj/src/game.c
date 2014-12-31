#include "game.h"
#include "frogger.h"
#include "keyboard.h"
#include "RTC.h"
#include "mouse.h"
#include "car.h"
#include "log.h"
#include "turtles.h"
#include "strings.h"
#include "colors.h"
#include <time.h>
#include <stdlib.h>
#include "graphics.h"


Game* newGame(){
	Game * jogo = (Game*) malloc(sizeof(Game));

	jogo->fundo = loadBitmap("/home/proj/res/test.bmp");
	jogo->carros = loadBitmap("/home/proj/res/car_sprites.bmp");
	jogo->troncos = loadBitmap("/home/proj/res/logs.bmp");
	jogo->camiao = loadBitmap("/home/proj/res/truck.bmp");
	jogo->tartarugas2 = loadBitmap("/home/proj/res/turtle_2_sprites.bmp");
	jogo->tartarugas3 = loadBitmap("/home/proj/res/turtle_3_sprites.bmp");
	jogo->toca = loadBitmap("/home/proj/res/toca.bmp");
	jogo->vidas = loadBitmap("/home/proj/res/lives.bmp");

	jogo->frog = newFrog();
	int i;
	for(i = 0;i < 5; i++){
		jogo->sapos[i] = 0;
	}


	jogo->lane1 = (Lane*)malloc(sizeof(Lane));
	jogo->lane1->size = 0;
	jogo->lane2 = (Lane*)malloc(sizeof(Lane));
	jogo->lane2->size = 0;
	jogo->lane3 = (Lane*)malloc(sizeof(Lane));
	jogo->lane3->size = 0;
	jogo->lane4 = (Lane*)malloc(sizeof(Lane));
	jogo->lane4->size = 0;
	jogo->lane5 = (Lane*)malloc(sizeof(Lane));
	jogo->lane5->size = 0;
	jogo->river1 = (RiverT*)malloc(sizeof(RiverT));
	jogo->river1->size = 0;
	jogo->river2 = (River*)malloc(sizeof(River));
	jogo->river2->size = 0;
	jogo->river3 = (River*)malloc(sizeof(River));
	jogo->river3->size = 0;
	jogo->river4 = (RiverT*)malloc(sizeof(RiverT));
	jogo->river4->size = 0;
	jogo->river5 = (River*)malloc(sizeof(River));
	jogo->river5->size = 0;

	srand(time(NULL));

	addCar(jogo->lane1,1);
	addCar(jogo->lane1,1);

	addCar(jogo->lane2,2);
	addCar(jogo->lane2,2);
	addCar(jogo->lane2,2);

	addCar(jogo->lane3,3);
	addCar(jogo->lane3,3);

	addCar(jogo->lane4,4);
	addCar(jogo->lane4,4);
	addCar(jogo->lane4,4);

	addCar(jogo->lane5,5);
	addCar(jogo->lane5,5);

	addTurtles(jogo->river1, 1);
	addTurtles(jogo->river1, 1);

	addLog(jogo->river2,2);
	addLog(jogo->river2,2);

	addLog(jogo->river3,3);
	addLog(jogo->river3,3);

	addTurtles(jogo->river4, 4);
	addTurtles(jogo->river4, 4);
	addTurtles(jogo->river4, 4);

	addLog(jogo->river5,5);
	addLog(jogo->river5,5);

	jogo->level = 1;
	jogo->lives = 3;
	jogo->pontos = 0;

	jogo->pause = 0;
	jogo->gameover = 0;
	jogo->done = 0;

	programDeltaAlarm(0,0,10);
	return jogo;
}
void updateGame(Game* game, unsigned long scanCode){
	switch(scanCode){
	case KEY_ESC:
		game->done = 1;
		break;
	case KEY_P:
		if(game->pause){
			game->pause = 0;
		}
		else{
			game->pause = 1;
		}
	}
	if(game->pause){
		return;
	}
	if(game->gameover == 3){
		game->done = 1;
		return;
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
		for(i = 0; i < game->lane5->size; i++){
			updateCar(game->lane5->cars[i]);
		}
		for(i = 0; i < game->river1->size; i++){
			updateTurtle(game->river1->t[i]);
		}
		for(i = 0; i < game->river2->size; i++){
			updateLog(game->river2->logs[i]);
		}
		for(i = 0; i < game->river3->size; i++){
			updateLog(game->river3->logs[i]);
		}
		for(i = 0; i < game->river4->size; i++){
			updateTurtle(game->river4->t[i]);
		}
		for(i = 0; i < game->river5->size; i++){
			updateLog(game->river5->logs[i]);
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
		case 5:
			collision = checkLaneCollisions(game->frog, game->lane5);
			break;
		case 7:
			collision = checkRiverTCollisions(game->frog, game->river1);
			break;
		case 8:
			collision = checkRiverCollisions(game->frog, game->river2);
			break;
		case 9:
			collision = checkRiverCollisions(game->frog, game->river3);
			break;
		case 10:
			collision = checkRiverTCollisions(game->frog, game->river4);
			break;
		case 11:
			collision = checkRiverCollisions(game->frog, game->river5);
			break;
		case 12:
			collision = checkSwampCollision(game->frog);
			if(collision == -1){
				collision = 1;
			}
			else{
				if(game->frog->vy == 0){
					game->frog->swamp = 1;
					game->sapos[collision] = 1;
					game->pontos += 100 * game->level;
				}
				collision = 0;
			}
			break;
		default:
			break;
		}
		if(collision){
			game->lives--;
			game->frog->dead = 1;
			if(!game->lives){
				game->gameover = 1;
			}
			game->frog->dir = 5;
		}

		int levelup = 1;
		for(i = 0; i < 5; i++){
			if(game->sapos[i] == 0){
				levelup = 0;
				break;
			}
		}
		if(levelup){
			game->level++;
			for(i = 0; i < 5; i++){
				game->sapos[i] = 0;
			}
		}
	}
}
void drawGame(Game* game){
	drawBitmap(game->fundo, 0, 0, ALIGN_LEFT);
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
	for(i = 0; i < game->lane5->size; i++){
		drawTruck(game->lane5->cars[i], game->camiao);
	}
	for(i = 0; i < game->river1->size; i++){
		drawTurtle(game->river1->t[i], game->tartarugas3);
	}
	for(i = 0; i < game->river2->size; i++){
		drawLog(game->river2->logs[i], game->troncos);
	}
	for(i = 0; i < game->river3->size; i++){
		drawLog(game->river3->logs[i], game->troncos);
	}
	for(i = 0; i < game->river4->size; i++){
		drawTurtle(game->river4->t[i], game->tartarugas2);
	}
	for(i = 0; i < game->river5->size; i++){
		drawLog(game->river5->logs[i], game->troncos);
	}
	for (i = 0; i < 5; i++){
		if(game->sapos[i]){
			drawBitmapT(game->toca, (0.22 + i * 0.1275)*get_h_res(), 0.1 *get_h_res(), ALIGN_LEFT);
		}
	}

	drawFrog(game->frog);
	drawString("lives", 0.05*get_h_res(), 0.0125*get_h_res(), green);
	drawString("points", 0.05*get_h_res(), 0.1125*get_h_res(), green);
	drawString("level", 0.85*get_h_res(), 0.0125*get_h_res(), green);

	int x = 0.05*get_h_res();
	for(i = 0; i < game->lives;i++){
		drawBitmapT(game->vidas, x,  0.0625 * get_h_res(), ALIGN_LEFT);
		x += 0.0375*get_h_res();
	}

	char string[15];

	sprintf(string, "%d", game->pontos);
	drawString(string, 0.05*get_h_res(), 0.1625*get_h_res(), green);

	sprintf(string, "%d", game->level);
	drawString(string, 0.85*get_h_res(), 0.0625 * get_h_res(), green);
	if(game->pause){
		drawString("pause", 0.45*get_h_res(), 0.45*get_v_res(), green);
	}

	if(game->gameover){
		drawString("game over", 0.45*get_h_res(), 0.45*get_v_res(), green);
	}

}
void deleteGame(Game* game){
	deleteBitmap(game->fundo);
	deleteBitmap(game->carros);
	deleteBitmap(game->troncos);
	deleteBitmap(game->camiao);
	deleteBitmap(game->tartarugas2);
	deleteBitmap(game->tartarugas3);
	deleteBitmap(game->toca);
	deleteBitmap(game->vidas);
	deleteFrog(game->frog);
	free(game->lane1);
	free(game->lane2);
	free(game->lane3);
	free(game->lane4);
	free(game->lane5);
	free(game->river1);
	free(game->river2);
	free(game->river3);
	free(game->river4);
	free(game->river5);
	free(game);
}

Frog* newFrog(){
	Frog * sapo = (Frog*) malloc(sizeof(Frog));

	sapo->img = loadBitmap("/home/proj/res/frog_sprites.bmp");
	sapo->dir = 1;
	sapo->anim = 0;

	sapo->swamp = 0;

	sapo->x = 0.475*get_h_res();
	sapo->y = 0.7*get_h_res();

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
			sapo->anim = 0;
			sapo->faixa = 0;
			sapo->dir = 1;
			sapo->x = 0.475*get_h_res();
			sapo->y = 0.7*get_h_res();
			sapo->dead = 0;
			sapo->vy = 0;
			sapo->vx = 0;
			return;
		default:
			break;
		}
	}
	if(getMouse()->leftButtonDown){
		if(!(getMouse()->x >= sapo->x && getMouse()->x <= sapo->x + 0.05*get_h_res() &&
				getMouse()->y >= sapo->y && getMouse()->y <= sapo->y + 0.05*get_h_res())) {
			int deltax = sapo->x + 0.025*get_h_res() - getMouse()->x;
			int deltay = sapo->y + 0.025*get_h_res() - getMouse()->y;
			if(abs(deltax) > abs(deltay)){
				if(deltax < 0){
					scanCode = KEY_ARR_RIGHT;
				}
				else{
					scanCode = KEY_ARR_LEFT;
				}
			}
			else{
				if(deltay < 0){
					scanCode = KEY_ARR_DOWN;
				}
				else{
					scanCode = KEY_ARR_UP;
				}
			}
		}
	}

	if((sapo->vx == 0 && sapo->vy == 0) || (sapo->faixa >= 7 && abs(sapo->vx) == (int)(40/getFPS() * 2))){
		switch(scanCode){
		case KEY_ARR_UP:
		case KEY_W :
			sapo->vx = 0;
			sapo->newX = 0;
			sapo->dir = 1;
			if(sapo->y - 0.05*get_h_res() < 0){
				sapo->newY = 0;
			}
			else{
				sapo->anim = 1;
				sapo->newY = sapo->y - 0.05*get_h_res();
				sapo->vy = - 5;
				sapo->faixa++;
			}
			break;
		case KEY_ARR_DOWN:
		case KEY_S:
			sapo->vx = 0;
			sapo->newX = 0;
			sapo->dir = 3;
			if(sapo->y + 0.05*get_h_res() > 0.7*get_h_res()){
				sapo->newY = 0.7*get_h_res();
			}
			else{
				sapo->newY = sapo->y + 0.05*get_h_res();
				sapo->anim = 1;
				sapo->vy = 5;
				sapo->faixa--;
			}
			break;
		case KEY_ARR_RIGHT:
		case KEY_D:
			sapo->vy = 0;
			sapo->newX = 0;
			sapo->dir = 2;
			if(sapo->x + 0.05*get_h_res() > 0.75*get_h_res()){
				sapo->newX = 0.75*get_h_res();
			}
			else{
				sapo->anim = 1;
				sapo->newX = sapo->x + 0.025*get_h_res();
				sapo->vx = 5;
			}
			break;
		case KEY_ARR_LEFT:
		case KEY_A:
			sapo->vy = 0;
			sapo->newX = 0;
			sapo->dir = 4;
			if(sapo->x - 0.05*get_h_res() < 0.2*get_h_res()){
				sapo->newX = 0.2*get_h_res();
			}
			else{
				sapo->anim = 1;
				sapo->newX = sapo->x - 0.025*get_h_res();
				sapo->vx = -5;
			}
			break;
		default:
			break;
		}
	}
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
	if(sapo->swamp){
		sapo->swamp = 0;
		sapo->anim = 0;
		sapo->faixa = 0;
		sapo->x = 0.475*get_h_res();
		sapo->dir = 1;
		sapo->y = 0.7*get_h_res();
		sapo->vy = 0;
		sapo->vx = 0;
		return;
	}
}

void drawFrog(Frog* sapo){

	int width = sapo->img->bitmapInfoHeader.width;
	int drawWidth = width/ 11;
	int height = sapo->img->bitmapInfoHeader.height;

	if (sapo->x + drawWidth < 0.2*get_h_res() || sapo->x > 0.8*get_h_res() || sapo->y + height < 0
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
	deleteBitmap(sapo->img);
	free(sapo);
}

int checkLaneCollisions(Frog * frog, Lane* lane){
	int i;
	for(i = 0;i < lane->size; i++){
		if(!(frog->x + 0.035*get_h_res() < lane->cars[i]->x || frog->x > lane->cars[i]->x + 0.035*get_h_res() ||
				frog->y + 0.035*get_h_res() < lane->cars[i]->y || frog->y > lane->cars[i]->y + 0.035*get_h_res()))
			return 1;
	}
	return 0;
}
int checkRiverCollisions(Frog * frog, River* river){
	if(frog->vy != 0){
		return 0;
	}
	int i;
	for(i = 0;i < river->size; i++){
		if(!(frog->x + 0.035*get_h_res() < river->logs[i]->x || frog->x + 0.035*get_h_res() > river->logs[i]->x + river->logs[i]->width ||
				frog->y + 0.035*get_h_res() < river->logs[i]->y || frog->y > river->logs[i]->y + 0.035*get_h_res())){
			if(frog->vx == 0 && frog->vy == 0){
				frog->vx = river->logs[i]->vx;
				if(frog->vx < 0){
					frog->newX = 0.2*get_h_res();
				}
				if(frog->vx > 0){
					frog->newX = 0.75*get_h_res();
				}
			}
			return 0;
		}
	}
	return 1;
}

int checkRiverTCollisions(Frog * frog, RiverT* river){
	if(frog->vy != 0){
		return 0;
	}
	int i;
	for(i = 0;i < river->size; i++){
		if(!(frog->x + 0.035*get_h_res() < river->t[i]->x || frog->x + 0.035*get_h_res() > river->t[i]->x + river->t[i]->width ||
				frog->y + 0.035*get_h_res() < river->t[i]->y || frog->y > river->t[i]->y + 0.035*get_h_res())){
			if(frog->vx == 0 && frog->vy == 0){
				frog->vx = river->t[i]->vx;
				if(frog->vx < 0){
					frog->newX = 0.2*get_h_res();
				}
				if(frog->vx > 0){
					frog->newX = 0.75*get_h_res();
				}
			}
			if(river->t[i]->anim){
				if(river->t[i]->desenho == 3){
					return 1;
				}
			}
			return 0;
		}
	}
	return 1;
}
int checkSwampCollision(Frog * frog){
	if(frog->vx != 0 || frog->vy != 0){
		return 0;
	}
	int i;
	if(!(frog->x < 0.2125*get_h_res() || frog->x + 0.05*get_h_res() > 0.28035*get_h_res()))
		return 0;
	if(!(frog->x < 0.3375*get_h_res() || frog->x + 0.05*get_h_res() > 0.40625*get_h_res()))
		return 1;
	if(!(frog->x < 0.4625*get_h_res() || frog->x + 0.05*get_h_res() > 0.53035*get_h_res()))
		return 2;
	if(!(frog->x < 0.5875*get_h_res() || frog->x + 0.05*get_h_res() > 0.65625*get_h_res()))
		return 3;
	if(!(frog->x < 0.7125*get_h_res() || frog->x + 0.05*get_h_res() > 0.78125*get_h_res()))
		return 4;
	return -1;
}

void addCar(Lane* lane, int faixa){

	if(lane->size == laneMaxSize){
		return;
	}
	int i;
	int colision = 0;
	Car* car = newCar(faixa);

	while(1){
		for(i=0; i < lane->size; i++){
			if(car->x >= lane->cars[i]->x - 0.05*get_h_res() && car->x <= lane->cars[i]->x + 0.1*get_h_res()){
				colision = 1 ;
				break;
			}
		}
		if(!colision){
			lane->cars[lane->size] = car;
			lane->size++;
			break;
		}
		deleteCar(car);
		car = newCar(faixa);
		colision = 0;
	}
}

void addLog(River* river, int faixa){

	if(river->size == riverMaxSize){
		return;
	}
	int i;
	int colision = 0;
	Log* log = newLog(faixa);

	while(1){
		for(i=0; i < river->size; i++){
			if(log->x + log->width  >= river->logs[i]->x - 0.05*get_h_res() &&
					log->x <= river->logs[i]->x + river->logs[i]->width + 0.05*get_h_res()){
				colision = 1 ;
				break;
			}
		}
		if(!colision){
			river->logs[river->size] = log;
			river->size++;
			break;
		}
		deleteLog(log);
		log = newLog(faixa);
		colision = 0;
	}
}

void addTurtles(RiverT* river, int faixa){

	if(river->size == riverMaxSize){
		return;
	}
	int i;
	int colision = 0;
	Turtles* turtle = newTurtle(faixa);

	while(1){
		for(i=0; i < river->size; i++){
			if(turtle->x + turtle->width  >= river->t[i]->x - 0.05*get_h_res() &&
					turtle->x <= river->t[i]->x + river->t[i]->width + 0.05*get_h_res()){
				colision = 1 ;
				break;
			}
		}
		if(!colision){
			river->t[river->size] = turtle;
			river->size++;
			break;
		}
		deleteTurtle(turtle);
		turtle = newTurtle(faixa);
		colision = 0;
	}
}
