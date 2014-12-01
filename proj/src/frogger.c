#include <minix/drivers.h>
#include "graphics.h"
#include "keyboard.h"
#include "mouse.h"
#include "frogger.h"
#include "timer.h"
#include "bitmap.h"

const int FRAMES_PER_SECOND = 30;
const int mouse_multiplier = 2;
Frogger* newFrogger(){

	//alocar espaco em memoria
	Frogger* frogger = (Frogger*)malloc(sizeof(frogger));

	// subscrever perifericos
	frogger->IRQ_KB = subscribe_kbd();
	frogger->IRQ_TIMER = subscribe_timer();
	frogger->IRQ_M = subscribe_mouse();

	frogger->timer = newTimer();

	newMouse();

	frogger->complete = 0; frogger->refresh = 1;
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
			if (msg.NOTIFY_ARG & frogger->IRQ_KB) {
				frogger->scanCode = kbc_read();
			}
			if (msg.NOTIFY_ARG & frogger->IRQ_TIMER) {
	 			timerHandler(frogger->timer);
			}
			if (msg.NOTIFY_ARG & frogger->IRQ_M) {
		    	updateMouse();
			}
 			break;
		default:
			break;
	  	}
	}
	if(frogger->scanCode != 0){
		if(frogger->scanCode == KEY_ESC){
			frogger->complete = 1;
		}
	}
	if(frogger->timer->ticked == 1){
		getMouse()->draw = 1;
	}
}


void drawFrogger(Frogger* frogger){
	Bitmap* fundo = loadBitmap("/home/proj/res/main.bmp");
	drawBitmap(fundo, 0, 0, ALIGN_LEFT);
}

void deleteFrogger(Frogger* frogger){
	//cancelar subscricoes
	unsubscribe_kbd();
	unsubscribe_mouse();
	unsubscribe_timer();

	free(frogger);
}
