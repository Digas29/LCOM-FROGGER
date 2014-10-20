#include "macros.h"
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>

int kbc_write(unsigned char cmd){
	unsigned long stat;
	int tries = 0;

	while(tries < 3) {
		sys_inb(STAT_REG, &stat); /* assuming it returns OK */
		/* loop while 8042 input buffer is not empty */
		if( (stat & IBF) == 0 ) {
			sys_outb(KBC_CMD_REG, cmd); /* no args command */
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		tries++;
	}
	return 1;
}

int kbc_read(){
	unsigned long stat;
	unsigned long data;
	int tries = 0;

	while(tries < 3) {

		if(sys_inb(STAT_REG, &stat) != OK)
		{
			return -1;
		}

		if(stat & OBF) {

			if ( (stat &(PAR_ERR | TO_ERR)) == 0 && sys_inb(OUT_BUF, &data) == OK)
				return data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		tries++;
	}
}
