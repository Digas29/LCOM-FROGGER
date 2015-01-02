#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "keyboard.h"

static unsigned int hookID_k; // Keyboard hook ID


int subscribe_kbd(void) {
	hookID_k = 1; // bit de subscricao
	int bitmask = BIT(hookID_k);

	if (sys_irqsetpolicy(IRQ_KBD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookID_k) != OK
			|| sys_irqenable(&hookID_k) != OK) {
		return -1;
	}

	return bitmask;
}

int unsubscribe_kbd() {
	return sys_irqdisable(&hookID_k) != OK || sys_irqrmpolicy(&hookID_k) != OK;
}

int kbc_read(){
	unsigned long stat;
	unsigned long data;
	int tries = 0;

	while(tries < 3) {

		if(sys_inb(STAT_REG, &stat) != OK)
		{
			return -1;
		}

		if(stat & OBF) {

			if ( (stat &(PAR_ERR | TO_ERR)) == 0 && sys_inb(OUT_BUF, &data) == OK){
				return data;
			}
			else{
				return -1;
			}
		}
		tries++;
	}
}

