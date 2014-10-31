#include "mouse.h"
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>


int mouse_read(){
	unsigned long data;
	if (sys_inb(DATA_PORT, &data) == OK)
		return data;
	else
		return -1;
}

int mouse_write_byte(unsigned char cmd){
	unsigned long status;
	do {
		if (sys_outb(CMD_PORT, WRITE_BYTE_MOUSE) != OK)	return 1;
		if (sys_outb(DATA_PORT, cmd) != OK)	return 1;
		tickdelay(WAIT_TICKS);
		status = mouse_read();
	}
	while(status != ACK);
	return 0;

}


