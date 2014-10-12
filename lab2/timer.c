#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdbool.h>

#include "i8254.h"

unsigned int counter = 0;
static unsigned int hookID = 0;


int timer_set_square(unsigned long timer, unsigned long freq) {
	if(timer <= 2 && freq > 0){
		unsigned long division = TIMER_FREQ / freq;
		unsigned char config;
		char timer_lsb,timer_msb;
		timer_lsb = (char) division;
		timer_msb = (char) (division >> 8);
		config = (TIMER_0 + timer) | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN;
		sys_outb(TIMER_0 + timer, timer_lsb);
		sys_outb(TIMER_0 + timer, timer_msb);
		return 0;
	}
	else {
		return 1;
	}
}

int timer_subscribe_int(void ) {
	unsigned int bit = hookID;
	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookID) != OK || sys_irqenable(&hookID) != OK){
		return -1;
	}
	return BIT(bit);
}

int timer_unsubscribe_int() {

	if(sys_irqrmpolicy(&hookID) != OK || sys_irqdisable(&hookID) != OK){
		return 1;
	}
	return 0;
}

void timer_int_handler() {
	counter++;
}

int timer_get_conf(unsigned long timer, unsigned char *st) {

	if(timer > 0 && timer < 2){
		unsigned long readBack = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_ ;
		sys_outb(TIMER_CTRL, readBack);
		unsigned long conf;
		sys_inb(TIMER_0 + timer, &conf);
		*st = (char)conf;
		return 0;
	}
	else {
		return 1;
	}
}

int timer_display_conf(unsigned char conf) {
	printf("Output = %d \n", conf >> 7);
	printf("Null = %d \n", (conf & BIT(6)) >> 6);
	printf("Counter initialization = %d \n", (conf >> 5) & (BIT(1) | BIT (0)));
	int mode = (conf >> 3) & (BIT(2) | BIT(1) | BIT (0));
	if( mode == 0) {
		printf("Programmed Mode =  Interrupt on terminal count \n");
	}
	else if (mode == 1){
		printf("Programmed Mode =  Hardware retriggerable one-shot \n");
	}
	else if (mode == 2 || mode == 6){
		printf("Programmed Mode =  Rate Generator \n");
	}
	else if (mode == 3 || mode == 7){
		printf("Programmed Mode =  Square Wave Generator \n");
	}
	else if (mode == 4){
		printf("Programmed Mode =  Software Triggered Strobe \n");
	}
	else if (mode == 5){
		printf("Programmed Mode =  Hardware Triggered Strobe \n");
	}
	int BDC = conf & BIT(0);
	if(BDC == 0) {
		printf("Format =  Binary \n");
	}
	else {
		printf("Format =  BCD \n");
	}
	return 0;
}

int timer_test_square(unsigned long freq) {
	
	return timer_set_square(0,freq);
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
						 timer_int_handler(); /* incrementar o numero de interrupçoes */
						 printf("Interrupcao efectuada com sucesso \n");
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
	 return 1;
}

int timer_test_config(unsigned long timer) {
	unsigned char conf;
	timer_get_conf(timer, &conf);
	timer_display_conf(conf);
	return 0;
}
