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
