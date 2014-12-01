#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdbool.h>
#include "timer.h"

static unsigned int hookID; // [0,31] escolher o bit de susbcricao

int subscribe_timer(void ) {
	hookID = 0;
	unsigned int bit = hookID; // bit de subscricao
	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookID) != OK || sys_irqenable(&hookID) != OK){
		return -1;
 	}
	return BIT(bit);
}

int unsubscribe_timer() {

	if(sys_irqrmpolicy(&hookID) != OK || sys_irqdisable(&hookID) != OK){
		return 1;
	}
	return 0;
}

Timer * newTimer(){
	Timer* timer = malloc(sizeof(Timer));

	timer->counter = 0;
	timer->ticked = 0;
	return timer;
}
void timerHandler(Timer * timer){
	timer->counter++;
	timer->ticked = 1;

}
void resetTimerFlag(Timer* timer){
	timer->ticked = 0;
}
void deleteTimer(Timer * timer){
	free(timer);
}
