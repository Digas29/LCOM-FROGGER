#include <minix/sysutil.h>
#include "test3.h"
#include "macros.h"

tickdelay(micros_to_ticks(DELAY_US);

int subscribe_kbd(void ) {
	unsigned int bit = hookID; // bit de subscricao
	if(sys_irqsetpolicy(IRQ_KBD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookID) != OK || sys_irqenable(&hookID) != OK){
		return -1;
	}
	return bit;
}

int unsubscribe_kbd() {

	if(sys_irqrmpolicy(&hookID) != OK || sys_irqdisable(&hookID) != OK){
		return 1;
	}
	return 0;
}

void kbd_handler() {

}

int kbd_test_scan(unsigned short ass); {
	 int ipc_status;
	 message msg;
	 int request;
	 int irq_set;

	 irq_set = BIT(subscribe_kbd());

	 if(ass == 0) {
		 while(1)) {
			 request = driver_receive(ANY, &msg, &ipc_status); /* Get a request message. */
			 if (request != 0 ) {
				 printf("driver_receive failed with: %d", request);
				 continue;
			 }
			 if (is_ipc_notify(ipc_status)) { /* received notification */
				 switch (_ENDPOINT_P(msg.m_source)) {
				 case HARDWARE: /* hardware interrupt notification */
					 if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */

					 }
					 break;
				 default:
					 break; /* no other notifications expected: do nothing */
				 }
			 }
			 else { /* received a standard message, not a notification */
				 /* no standard messages expected: do nothing */
			 }
		 }
		 unsubscribe_int();
		 return 0;
	 }
	 return 1;
}
int kbd_test_leds(unsigned short n, unsigned short *leds) {
    /* To be completed */
}
int kbd_test_timed_scan(unsigned short n) {
    /* To be completed */
}
