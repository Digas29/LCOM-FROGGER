#include <minix/drivers.h>
#include <stdlib.h>
#include "graphics.h"
#include "keyboard.h"
#include "mouse.h"
#include "frogger.h"
#include "timer.h"
#include "bitmap.h"
#include "path.h"
#include "game.h"
#include "strings.h"
#include "settings.h"
#include "RTC.h"
#include "highscores.h"

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

	frogger->IRQ_R = subscribe_RTC();

	frogger->estado = MAIN_MENU;
	frogger->state = newMainMenu();

	frogger->draw = 1;

	frogger->up = 0;


	newMouse();
	newTimer();
	newAlphabet();
	newRecords();
	loadRecords();

	frogger->complete = 0;
	frogger->scanCode = 0;
	return frogger;
}

void updateFrogger(Frogger* frogger){
	int ipc_status,r;
	message msg;

	resetTimerFlag();

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
				if(frogger->scanCode == 0xE0){
					frogger->scanCode = (0xE0 << 8) | kbc_read();
				}
				else{
					frogger->scanCode = kbc_read();
				}
			}
			if (msg.NOTIFY_ARG & frogger->IRQ_TIMER) {
				timerHandler();
			}
			if(msg.NOTIFY_ARG & frogger->IRQ_R){
				RTC_IH();
			}
			else
 			break;
		default:
			break;
	  	}
	}
	if(getTimer()->ticked){
		getMouse()->draw = 1;
		frogger->draw = 1;
	}
	if(getTimer()->counter % mouse_multiplier == 0){
		switch(frogger->estado){
	 	case MAIN_MENU:
			updateMainMenu(frogger->state, frogger->scanCode);
			break;
		case GAME:
			if(((Game*)frogger->state)->frog->dead){
				if(getTimer()->counter % 30 == 0){
					updateGame(frogger->state, frogger->scanCode);
					if(((Game*)frogger->state)->gameover){
						((Game*)frogger->state)->gameover++;
					}
				}
			}
			else if(((Game*)frogger->state)->alarm){
				if(getTimer()->counter % 30 == 0){
					updateGame(frogger->state, frogger->scanCode);
					((Game*)frogger->state)->alarm++;
				}
			}
			else{
				updateGame(frogger->state, frogger->scanCode);
			}
			break;
		case SETTINGS:
			updateSettingsMenu(frogger->state, frogger->scanCode);
			break;
		case HIGHSCORES_MENU:
			updateHighScoresMenu(frogger->state, frogger->scanCode);
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
		if(frogger->estado == SETTINGS){
			drawSettingsMenu((SettingsMenu*)frogger->state);
		}
		if(frogger->estado == HIGHSCORES_MENU){
			drawHighScoresMenu((HighScoresMenu*)frogger->state);
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
	unsubscribe_mouse();
	unsubscribe_timer();


	free(frogger);
}
void deleteState(Frogger* frogger){
	switch(frogger->estado){
	case MAIN_MENU:
		deleteMainMenu((MainMenu*)frogger->state);
		break;
	case GAME:
		deleteGame((Game*)frogger->state);
		break;
	case SETTINGS:
		deleteSettingsMenu((SettingsMenu*)frogger->state);
		break;
	case HIGHSCORES_MENU:
		deleteHighScoresMenu((HighScoresMenu*)frogger->state);
		break;
	default:
		break;
	}
}

void changeState(Frogger* frogger, State newSate){
	deleteState(frogger);

	frogger->estado = newSate;
	switch(frogger->estado){
	case MAIN_MENU:
		frogger->state = newMainMenu();
		break;
	case GAME:
		frogger->state = newGame();
		break;
	case SETTINGS:
		frogger->state = newSettingsMenu();
		break;
	case HIGHSCORES_MENU:
		frogger->state = newHighScoresMenu();
		break;
	default:
		break;
	}
 }

void updateState(Frogger* frogger){
	switch(frogger->estado){
	case MAIN_MENU:
		if(((MainMenu*)frogger->state)->done){
			if(((MainMenu*)frogger->state)->mousePlay){
				getMouse()->leftButtonDown = 0;
				changeState(frogger,GAME);
			}
			else if(((MainMenu*)frogger->state)->mouseSettings){
				getMouse()->leftButtonDown = 0;
				changeState(frogger, SETTINGS);
			}
			else if(((MainMenu*)frogger->state)->mouseHighScores){
				getMouse()->leftButtonDown = 0;
				changeState(frogger, HIGHSCORES_MENU);
			}
			else if(((MainMenu*)frogger->state)->mouseExit){
				getMouse()->leftButtonDown = 0;
				frogger->complete = 1;
			}
		}
		break;
	case GAME:
		if(((Game*)frogger->state)->done){
			changeState(frogger,MAIN_MENU);
		}
		break;
	case SETTINGS:
		if(((SettingsMenu*)frogger->state)->done){
			if(((SettingsMenu*)frogger->state)->mode114){
				if(get_h_res() != 800){
					vg_exit();
					vg_init(0x114);
					deleteSettingsMenu((SettingsMenu*)frogger->state);
					frogger->state = newSettingsMenu();
				}
				((SettingsMenu*)frogger->state)->done = 0;
				((SettingsMenu*)frogger->state)->mode114 = 0;
				getMouse()->leftButtonDown = 0;
 	 		}
	  		else if(((SettingsMenu*)frogger->state)->mode117){
				if(get_h_res() != 1024){
					vg_exit();
					vg_init(0x117);
					deleteSettingsMenu((SettingsMenu*)frogger->state);
					frogger->state = newSettingsMenu();
				}
				((SettingsMenu*)frogger->state)->done = 0;
				((SettingsMenu*)frogger->state)->mode117 = 0;
				getMouse()->leftButtonDown = 0;
			}
			else if(((SettingsMenu*)frogger->state)->mouseExit){
				getMouse()->leftButtonDown = 0;
				changeState(frogger,MAIN_MENU);
			}
		}
		break;
	case HIGHSCORES_MENU:
		if(((HighScoresMenu*)frogger->state)->mouseExit){
			getMouse()->leftButtonDown = 0;
			changeState(frogger,MAIN_MENU);
		}
		break;
	default:
		break;
	}
}
