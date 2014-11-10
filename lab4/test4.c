#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "test4.h"
#include "mouse.h"
#include "kbc.h"

unsigned int hookID;
unsigned int hookID_t;
unsigned long packet[3];
unsigned int counter;
unsigned int time;
int X,Y;

int subscribe_mouse(void) {
	hookID = 12;
	unsigned long bitmask = BIT(hookID);

	mouse_write_byte(DISABLE_DATA_PACKETS);

	if (sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookID) != OK
			|| sys_irqenable(&hookID) != OK) {
		return -1;
	}
	mouse_write_byte(DATA_PACKETS);
	mouse_write_byte(STREAM_MODE);
	return bitmask;
}

int unsubscribe_mouse() {
	mouse_write_byte(DISABLE_DATA_PACKETS);
	return sys_irqdisable(&hookID) != OK || sys_irqrmpolicy(&hookID) != OK;
}
int timer_subscribe(void) {
	hookID_t = 0;
	unsigned int bitmask = BIT(hookID_t); // bit de subscricao
	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookID_t) != OK || sys_irqenable(&hookID_t) != OK){
		return -1;
	}
	return bitmask;
}

int timer_unsubscribe() {

	if(sys_irqrmpolicy(&hookID_t) != OK || sys_irqdisable(&hookID_t) != OK){
		return 1;
	}
	return 0;
}

void packet_print(){
	short x = (X_SIGN(packet[0]) ? (packet[1] | (0xff << 8)) : packet[1]);
	short y = (Y_SIGN(packet[0]) ? (packet[2] | (0xff << 8)) : packet[2]);
	printf("B1=0x%x B2=0x%x B3=0x%x LB=%d MB=%d RB=%d XOV=%d YOV=%d X=%d Y=%d \n",
			packet[0], packet[1], packet[2],
			LB(packet[0]), MB(packet[0]), RB(packet[0]),
			X_OVF(packet[0]), Y_OVF(packet[0]),
			x, y);
	counter = 0;
}
void packet_sum(){
	short x = (X_SIGN(packet[0]) ? (packet[1] | (0xff << 8)) : packet[1]);
	short y = (Y_SIGN(packet[0]) ? (packet[2] | (0xff << 8)) : packet[2]);
	if(LB(packet[0])){
		X += x;
		Y += y;
		printf("X: %d	LB:%d \n",X,LB(packet[0]));
	}
	else{
		X=0;
		Y=0;
		printf("LB:%d \n",LB(packet[0]));
	}
	counter = 0;
}

int mouse_handler(int gesture) {
	unsigned long byte;
	int tries = 0;
	if(counter == 0){
		while(tries < 3){
			byte = mouse_read();
			if(byte & BIT(3)){
				packet[0] = byte;
				break;
			}
		}
		counter++;
		return 0;
	}
	else{
		byte = mouse_read();
		packet[counter] = byte;
		counter++;
		if(counter == 3){
			counter = 0;
			time = 0;
			if(!gesture)
				packet_print();
			else
				packet_sum();
		}
		return 0;
	}
	return 1;
}

int test_packet(unsigned short cnt){

	int j = 0;
	int ipc_status;
	message msg;
	int request;
	unsigned long  irq_set;

	counter = 0;
	X=0;
	Y=0;

	irq_set = subscribe_mouse();

	while(j < 3 * cnt) {
		request = driver_receive(ANY, &msg, &ipc_status);
		if (request != 0 ) {
			printf("driver_receive failed with: %d", request);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					mouse_handler(0);
					j++;
				}
				break;
			default:
				break;
			}
		}
	}
	mouse_write_byte(DISABLE_DATA_PACKETS);
	unsubscribe_mouse();
	printf("%d", cnt);
	printf(" packets printed\n");
	return 0;
}

int test_async(unsigned short idle_time) {

	int ipc_status;
	message msg;
	int request;
	unsigned long  irq_set1, irq_set2;

	time = 0;
	counter = 0;

	irq_set1 = subscribe_mouse();
	irq_set2 = timer_subscribe();

	while(time < idle_time * 60) {
		request = driver_receive(ANY, &msg, &ipc_status);
		if (request != 0 ) {
			printf("driver_receive failed with: %d", request);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set1) {
					mouse_handler(0);
				}
				if(msg.NOTIFY_ARG & irq_set2){
					time++;
				}
				break;
			default:
				break;
			}
		}
	}
	unsubscribe_mouse();
	timer_unsubscribe();
	printf("\nLimit time exceed!\n");
	return 0;
}

int test_config(void) {
	unsigned long byte;
	subscribe_mouse();
	mouse_write_byte(STATUS_REQUEST);
	byte = mouse_read();
	if(byte == -1) return 1;
	printf("BYTE 1: 0x%X\n", byte);
	printf("Scaling: ");
	if(SCALING(byte))
		printf("2:1  ");
	else
		printf("1:1  ");
	printf("Data Reporting: ");
	if(!DATA_REPORTING(byte))
		printf("disable  ");
	else
		printf("enable   ");
	printf("Mode: ");
	if(!MODE(byte))
		printf("remote mode\n\n");
	else
		printf("stream mode\n\n");
	byte = mouse_read();
	if(byte == -1) return 1;
	printf("byte 2: 0x%X\n", byte);
	printf("Resolution: %d\n\n", byte);
	byte = mouse_read();
	if(byte == -1) return 1;
	printf("byte 3: 0x%X\n", byte);
	printf("Sample Rate: %d\n\n", byte);
}


int test_gesture(short length, unsigned short tolerance) {
	int ipc_status;
	message msg;
	int request;
	unsigned long  irq_set;

	counter = 0;
	X = 0;
	Y = 0;

	irq_set = subscribe_mouse();

	while(X < length) {
		request = driver_receive(ANY, &msg, &ipc_status);
		if (request != 0 ) {
			printf("driver_receive failed with: %d", request);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					mouse_handler(1);
					if(abs(Y) > tolerance){
						X=0; Y=0;
					}
				}
				break;
			default:
				break;
			}
		}
	}
	mouse_write_byte(DISABLE_DATA_PACKETS);
	unsubscribe_mouse();
	printf("\nExit gesture!\n");
	return 0;

}
