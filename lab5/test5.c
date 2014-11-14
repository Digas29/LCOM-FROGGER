#include "keyboard.h"
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>


unsigned long scanCode;

void *test_init(unsigned short mode, unsigned short delay) {
	vg_init(mode);
	sleep(delay);
	vg_exit();
	
}
void kbd_handler() {
	scanCode = kbc_read();
}


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	vg_init(0x105);

	int ipc_status;
	message msg;
	int request;
	int irq_set;

	irq_set = subscribe_kbd();

	while (scanCode != EXIT_BREAK_CODE) {
		request = driver_receive(ANY, &msg, &ipc_status);
		if (request != 0) {
			printf("driver_receive failed with: %d", request);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					kbd_handler();
				}
				vg_draw_rectangle(0,0,100,100,50);
				break;
			default:
				break;
			}
		}
		else {
		}
	}
	unsubscribe_kbd();
	vg_exit();
	return 0;
}

int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
	
	/* To be completed */
	
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	
	/* To be completed */
	
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time) {
	
	/* To be completed */
	
}					


