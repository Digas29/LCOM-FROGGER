#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8254.h"
int timer_set_square(unsigned long timer, unsigned long freq) {

	return 1;
}

int timer_subscribe_int(void ) {

	return 1;
}

int timer_unsubscribe_int() {

	return 1;
}

void timer_int_handler() {

}

int timer_get_conf(unsigned long timer, unsigned char *st) {

	if(timer > 0 && timer < 2){
		unsigned long read_back = TIMER_RB_CMD | TIMER_RB_SEL(timer);
		sys_outb(TIMER_CTRL, read_back);
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
	printf("Null = %d \n", (conf >> 6) & BIT(0));
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
	else if (mode == 3 || Mode = 7){
			printf("Programmed Mode =  Square Wave Generator \n");
	}
	else if (mode == 4){
				printf("Programmed Mode =  Software Triggered Strobe \n");
	}
	else if (mode == 5){
					printf("Programmed Mode =  Hardware Triggered Strobe \n");
		}
	int BDC = conf & BIT(0) ;
	if(BDC == 0) {
		printf("Format =  Binary \n");
	}
	else {
		printf("Format =  BDC \n");
	}
	return 0;
}

int timer_test_square(unsigned long freq) {
	
	return 1;
}

int timer_test_int(unsigned long time) {

	return 1;
}

int timer_test_config(unsigned long timer) {
	/**
	 * @brief Tests display of timer config
	 *
	 * Just calls timer_get_conf() followed by timer_display_conf()
	 *
	 * @param timer Timer whose config to read (Ranges from 0 to 2)
	 * @return Return 0 upon success and non-zero otherwise
	 */
	unsigned char conf;
	timer_get_conf(timer, &conf);
	timer_display_conf(conf);
	return 0;
}
