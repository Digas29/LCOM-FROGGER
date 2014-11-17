#include "keyboard.h"
#include "timer.h"
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>


unsigned long scanCode;
unsigned int counter;


void kbd_handler() {
	scanCode = kbc_read();
}

void timer_handler(){
	counter++;
}

void *test_init(unsigned short mode, unsigned short delay) {
	char adress = vg_init(mode);

	int ipc_status;
	message msg;
	int request;
	int irq_set;
	counter = 0;

	irq_set = subscribe_timer();

	while (counter < delay*60) {
		request = driver_receive(ANY, &msg, &ipc_status);
		if (request != 0) {
			printf("driver_receive failed with: %d", request);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					timer_handler();
				}
				break;
			default:
				break;
			}
		}
		else {
		}
	}
	unsubscribe_timer();
	vg_exit();
	printf("0x%X \n",adress);
	return 0;
	
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
				vg_draw_rectangle(x,y,size,size,color);
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
				vg_draw_line(xi,xf,yi,yf,color);
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

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	
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
				vg_draw_xpm(xi,yi, xpm);
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

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time) {

	vg_init(0x105);

	int ipc_status;
	message msg;
	int request;
	int irq_set1,irq_set2;
	int velocidade= delta/time;

	counter = 0;
	irq_set1 = subscribe_kbd();
	irq_set2 = subscribe_timer();

	while (scanCode != EXIT_BREAK_CODE) {
		request = driver_receive(ANY, &msg, &ipc_status);
		if (request != 0) {
			printf("driver_receive failed with: %d", request);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set1) {
					kbd_handler();
				}
				if (msg.NOTIFY_ARG & irq_set2) {
					timer_handler();
					if(counter%60 == 0){
						vg_draw_xpm(xi,yi,xpm);
						if(!hor){
							xi+=velocidade;
						}
						else{
							yi+=velocidade;
						}
					}
					if(counter== time*60){
						unsubscribe_timer();
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
	vg_exit();
	return 0;
	
}
