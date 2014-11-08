#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "test4.h"
#include "mouse.h"
#include "kbc.h"

static unsigned int hookID;
unsigned long packet[3];
unsigned int counter = 0;

int subscribe_mouse(void) {
	hookID = 1; // bit de subscricao
	unsigned long bitmask = BIT(hookID);

	if (sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookID) != OK
			|| sys_irqenable(&hookID) != OK) {
		return -1;
	}

	return bitmask;
}

int unsubscribe_mouse() {
	return sys_irqdisable(&hookID) != OK || sys_irqrmpolicy(&hookID) != OK;
}

void packet_print(){
	short X = (X_SIGN(packet[0]) ? (~(packet[1])+ 1) : packet[1]);
	short Y = (Y_SIGN(packet[0]) ? (~(packet[2])+ 1) : packet[2]);
	printf("B1=0x%x B2=0x%x B3=0x%x LB=%d MB=%d RB=%d XOV=%d YOV=%d X=%d Y=%d \n",
			packet[0], packet[1], packet[2],
			LB(packet[0]), MB(packet[0]), RB(packet[0]),
			X_OVF(packet[0]), Y_OVF(packet[0]),
			X, Y);
}

int mouse_handler() {
	if(counter < 3){
		packet[counter] = mouse_read();
		counter++;
		return 0;
	}
	else{
		counter = 0;
		return 1;
	}
}

int test_packet(unsigned short cnt){

	int ipc_status;
	message msg;
	int request;
	unsigned long  irq_set;

	irq_set = subscribe_mouse();
	mouse_write_byte(STREAM_MODE);

	while(cnt) {
		request = driver_receive(ANY, &msg, &ipc_status);
		if (request != 0 ) {
			printf("driver_receive failed with: %d", request);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					if(mouse_handler() == 1){
						packet_print();
						cnt--;
					}
				}
				break;
			default:
				break;
			}
		}
	}
	mouse_write_byte(DISABLE_STREAM_MODE);
	unsubscribe_mouse();
	return 0;
}	

int test_async(unsigned short idle_time) {
	/* To be completed ... */
}

int test_config(void) {
	unsigned long byte;
	subscribe_mouse();
	mouse_write_byte(STREAM_MODE);
	mouse_write_byte(STATUS_REQUEST);
	byte = mouse_read();
	printf("BYTE 1: 0x%X\n", byte);
	printf("Scaling: ");
	if(!SCALING(byte))
		printf("1:1  ");
	else
		printf("2:1  ");
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
	printf("BYTE 2: 0x%X\n", byte);
	printf("Resolution: %d\n\n", byte);
	byte = mouse_read();
	printf("BYTE 3: 0x%X\n", byte);
	printf("Sample Rate: %d\n\n", byte);
	mouse_write_byte(DISABLE_STREAM_MODE);
}


int test_gesture(short length, unsigned short tolerance) {
	/* To be completed ... */
}
