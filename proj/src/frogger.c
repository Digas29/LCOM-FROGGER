#include <minix/drivers.h>
#include "graphics.h"
#include "keyboard.h"
#include "mouse.h"
#include "frogger.h"
#include "timer.h"
#include "bitmap.h"
#include "game.h"
#include "strings.h"

const int FRAMES_PER_SECOND = 30;
const int mouse_multiplier = 2;

const int getFPS(){
	return FRAMES_PER_SECOND;
}

Frogger* newFrogger(){

	//alocar espaco em memoria
	Frogger* frogger = (Frogger*)malloc(sizeof(frogger));

	// subscrever perifericos
	frogger->IRQ_KB = subscribe_kbd();
	frogger->IRQ_TIMER = subscribe_timer();
	frogger->IRQ_M = subscribe_mouse();

	frogger->estado = GAME;
	frogger->state = newGame();
	frogger->draw = 1;

	frogger->timer = newTimer();
	frogger->up = 0;

	newMouse();
	newAlphabet();

	frogger->complete = 0;
	frogger->refresh = 1;
	frogger->scanCode = 0;

	return frogger;
}

void updateFrogger(Frogger* frogger){
	int ipc_status,r;
	message msg;

	resetTimerFlag(frogger->timer);

	if (driver_receive(ANY, &msg, &ipc_status) != 0 ) {
		return;
	}
	if (is_ipc_notify(ipc_status)) {
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE:
			if (msg.NOTIFY_ARG & frogger->IRQ_M) {
				updateMouse();
			}
			if (msg.NOTIFY_ARG & frogger->IRQ_KB) {
				frogger->scanCode = kbc_read();
			}
			if (msg.NOTIFY_ARG & frogger->IRQ_TIMER) {
				timerHandler(frogger->timer);
			}
			else
 			break;
		default:
			break;
	  	}
	}
	if(frogger->timer->ticked){
		getMouse()->draw = 1;
		frogger->draw = 1;
	}
	if(frogger->timer->counter % mouse_multiplier == 0){
		switch(frogger->estado){
	 	case MAIN_MENU:
			updateMainMenu(frogger->state, frogger->scanCode);
			break;
		case GAME:
			if(((Game*)frogger->state)->frog->dead){
				if(frogger->timer->counter % 20 == 0){
					updateGame(frogger->state, frogger->scanCode);
				}
			}
			else if(((Game*)frogger->state)->gameover){
				if(frogger->timer->counter % 20 == 0){
					updateGame(frogger->state, frogger->scanCode);
					((Game*)frogger->state)->gameover++;
				}
				if(((Game*)frogger->state)->gameover == 2){
					((Game*)frogger->state)->done = 1;
				}
			}
			else{
				updateGame(frogger->state, frogger->scanCode);
			}
			break;
		default:
			break;
		}
		updateState(frogger);
		frogger->up = 1;
	}
}


void drawFrogger(Frogger* frogger){
	if(frogger->up){
		if(frogger->estado == MAIN_MENU){
			drawMainMenu((MainMenu *)frogger->state);
		}
		if(frogger->estado == GAME){
			drawGame((Game*)frogger->state);
		}
		frogger->up = 0;
	}
	flipMouseBuffer();
	if(getMouse()->draw){
		drawMouse();
	}
	flipVRAM();
	frogger->draw = 0;
}

void deleteFrogger(Frogger* frogger){
	//cancelar subscricoes

	unsubscribe_kbd();
	//unsubscribe_mouse();
	unsubscribe_timer();


	free(frogger);
}

void changeState(Frogger* frogger, State newSate){
	//deleteState(frogger);

	frogger->estado = newSate;
	switch(frogger->estado){
	case MAIN_MENU:
		frogger->state = newMainMenu();
		break;
	case GAME:
		frogger->state = newGame();
		break;
	default:
		break;
	}
}

void updateState(Frogger* frogger){
	switch(frogger->estado){
	case MAIN_MENU:
		if(((MainMenu*)frogger->state)->done){
			if((*(MainMenu*)frogger->state).mousePlay){
				changeState(frogger,GAME);
			}
			if((*(MainMenu*)frogger->state).mouseExit){
				frogger->complete = 1;
			}
		}
		break;
	case GAME:
		if(((Game*)frogger->state)->done){
			frogger->complete = 1;
		}
		break;
	default:
		break;
	}
}
void deleteState(Frogger* frogger){
	switch(frogger->estado){
	case MAIN_MENU:
		deleteMainMenu((MainMenu*)frogger->state);
		break;
	case GAME:
		deleteGame((Game*)frogger->state);
		break;
	default:
		break;
	}
}
