#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdbool.h>
#include <stdlib.h>
#include "timer.h"

static unsigned int hookID_t; // [0,31] escolher o bit de susbcricao
Timer* timer = NULL;

int subscribe_timer(void ) {
	hookID_t = 0;
	unsigned int bit = hookID_t; // bit de subscricao
	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookID_t) != OK || sys_irqenable(&hookID_t) != OK){
		return -1;
 	}
	return BIT(bit);
}

int unsubscribe_timer() {

	if(sys_irqrmpolicy(&hookID_t) != OK || sys_irqdisable(&hookID_t) != OK){
		return 1;
	}
	return 0;
}

void newTimer(){
	Timer* t = malloc(sizeof(Timer));

	t->counter = 0;
	t->ticked = 0;
	timer = t;
}
void timerHandler(){
	timer->counter++;
	timer->ticked = 1;

}
void resetTimerFlag(){
	timer->ticked = 0;
}
void deleteTimer(){
	free(timer);
	timer = NULL;
}
Timer * getTimer(){
	return timer;
}
