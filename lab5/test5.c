#include "keyboard.h"
#include "timer.h"
#include "video_gr.h"
#include "test5.h"
#include "sprite.h"
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>

#define FPS 60
#define round(x) (x<0?ceil((x)-0.5):floor((x)+0.5))
#define GRAPHIC_MODE 0x105

unsigned long scanCode;
unsigned int counter;

phys_bytes vram_phisical_address;

void kbd_handler() {
	scanCode = kbc_read();
}

void timer_handler(){
	counter++;
}

void *test_init(unsigned short mode, unsigned short delay) {
	vg_init(mode);

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
	printf("VRAM physical address: 0x%x\n",vram_phisical_address);
	return 0;
	
}

int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	vg_init(GRAPHIC_MODE);

	int ipc_status;
	message msg;
	int request;
	int irq_set;

	irq_set = subscribe_kbd();

	while (scanCode != EXIT_MAKE_CODE) {
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
	vg_init(GRAPHIC_MODE);

	int ipc_status;
	message msg;
	int request;
	int irq_set;

	irq_set = subscribe_kbd();

	while (scanCode != EXIT_MAKE_CODE) {
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
	
	vg_init(GRAPHIC_MODE);

	int ipc_status;
	message msg;
	int request;
	int irq_set;

	irq_set = subscribe_kbd();

	while (scanCode != EXIT_MAKE_CODE) {
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

	int ipc_status;
	message msg;
	int request;
	int irq_set1,irq_set2;
	vg_init(GRAPHIC_MODE);
	Sprite*sprite = create_sprite(xpm);
	if(!hor){
		sprite->xspeed = ((double)delta/(double)time)/FPS;
		sprite->yspeed = 0;
	}
	else{
		sprite->xspeed = 0;
		sprite->yspeed =  ((double)delta/(double)time)/FPS;
	}
	sprite->x = xi;
	sprite->y = yi;

	draw_sprite(sprite);

	counter = 0;
	irq_set1 = subscribe_kbd();
	irq_set2 = subscribe_timer();


	while (scanCode != EXIT_MAKE_CODE) {
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
					if(counter % (int)(60/FPS) == 0){

						update_sprite(sprite);
						draw_sprite(sprite);
					}
					if(counter == time * 60){
						unsubscribe_timer();
					}
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
	unsubscribe_kbd();
	vg_exit();
	return 0;
	
}
int test_controller(){
	controler_info();
	return 0;
}
