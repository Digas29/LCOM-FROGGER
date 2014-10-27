#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "test3.h"
#include "macros.h"
#include "kbc.h"

static unsigned int hookID; // Keyboard hook ID
static unsigned int special;
static unsigned char ledStatus;
unsigned long scanCode;

static unsigned int counter;
static unsigned int hookIDt; // [0,31] escolher o bit de susbcricao TIMER0



int timer_subscribe_int(void ) {
	hookIDt = 0;
	unsigned int bit = hookIDt; // bit de subscricao
	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookIDt) != OK || sys_irqenable(&hookIDt) != OK){
		return -1;
	}
	return BIT(bit);
}

int timer_unsubscribe_int() {

	if(sys_irqrmpolicy(&hookIDt) != OK || sys_irqdisable(&hookIDt) != OK){
		return 1;
	}
	return 0;
}

void timer_int_handler() {
	counter++;
}
int timer_test_int(unsigned long time) {

	int ipc_status;
	message msg;
	int request;
	int irq_set;

	irq_set = timer_subscribe_int();

	if(time > 0) {
		while(counter < time * 60) {
			request = driver_receive(ANY, &msg, &ipc_status); /* Get a request message. */
			if (request != 0 ) {
				printf("driver_receive failed with: %d", request);
				continue;
			}
			if (is_ipc_notify(ipc_status)) { /* received notification */
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
						timer_int_handler(); /* incrementar contador*/
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
		timer_unsubscribe_int();
		return 0;
	}
}
int subscribe_kbd(void) {
	hookID = IRQ_KBD; // bit de subscricao
	int bitmask = BIT(hookID);

	if (sys_irqsetpolicy(IRQ_KBD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookID) != OK
			|| sys_irqenable(&hookID) != OK) {
		return -1;
	}

	return bitmask;
}

int unsubscribe_kbd() {
	return sys_irqdisable(&hookID) != OK || sys_irqrmpolicy(&hookID) != OK;
}

int print_code(){
	switch (scanCode){
	case -1:
		return 1;
		break;
	case EXIT_BREAK_CODE:
		printf("Brakecode: 0x%x\n", scanCode);
		printf("ESC KEY pressed \n");
		return 1;
		break;
	case SPECIAL_KEY:
		special = 1;
		break;
	default:
		break;
	}

	switch (special){
	case 0:
		if (scanCode & BIT(7)) {
			printf("Brakecode: 0x%x\n", scanCode);
		}
		else {
			printf("Makecode: 0x%x\n", scanCode);
			counter = 0;
		}
		break;
	case 1:
		special++;
		break;
	case 2:
		if (scanCode & BIT(7)) {
			printf("Brakecode: 0xe0%x\n", scanCode);
			special = 0;
		}
		else {
			printf("Makecode: 0xe0%x\n", scanCode);
			special = 0;
			counter = 0;
		}
		break;
	default:
		break;
	}
	return 0;
}

int kbd_handler() {
	scanCode = kbc_read();
	return print_code();

}

int kbd_test_scan(unsigned short ass) {
	int ipc_status;
	message msg;
	int request;
	int irq_set;

	int stop = 0;

	irq_set = subscribe_kbd();

	while (!stop) {
		request = driver_receive(ANY, &msg, &ipc_status);
		if (request != 0) {
			printf("driver_receive failed with: %d", request);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					if(!ass){
						stop = kbd_handler();
					}
					else{
						kbd_handler_asm();
						stop = print_code();
					}
				}
				break;
			default:
				break;
			}
		}
		else {
		}
	}
	unsubscribe_kbd();
	return 0;
}
int kbd_test_leds(unsigned short n, unsigned short *leds) {

	unsigned long status;
	int i = 0;

	ledStatus = 0;
	for(i = 0; i < n; i++){
		kbc_write(LEDS);
		status = kbc_read();
		ledStatus ^= BIT(leds[i]);
		kbc_write(ledStatus);
		status = kbc_read();
		switch (status) {
		case RESEND_LEDS:
			kbc_write(ledStatus);
			break;
		case ERROR_LEDS:
			kbc_write(LEDS);
			kbc_write(ledStatus);
			break;
		default:
			break;
		}
		timer_test_int(1);
	}
	return 0;
}
int kbd_test_timed_scan(unsigned short n) {
	int ipc_status;
	message msg;
	int request;
	int irq_set1;
	int irq_set2;

	int stop = 0;
	counter = 0;

	irq_set1 = subscribe_kbd();
	irq_set2 = timer_subscribe_int();


	while (!stop && counter < 60 *n) {
		request = driver_receive(ANY, &msg, &ipc_status);
		if (request != 0) {
			printf("driver_receive failed with: %d", request);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set1) {
					stop = kbd_handler();
				}
				if (msg.NOTIFY_ARG & irq_set2) {
					timer_int_handler();
				}
				break;
			default:
				break;
			}
		}
		else {
		}
	}
	printf("The time limit was exceded\n");
	unsubscribe_kbd();
	timer_unsubscribe_int();
	return 0;

}
