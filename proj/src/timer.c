#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdbool.h>
#include "timer.h"

static unsigned int hookID_t; // [0,31] escolher o bit de susbcricao

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
