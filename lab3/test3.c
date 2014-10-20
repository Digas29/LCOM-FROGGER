#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "test3.h"
#include "macros.h"
#include "kbc.h"

static unsigned int hookID;

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

int kbd_handler() {
	unsigned long code = kbc_read();

	if (code & BIT(7)) {
		printf("Brakecode: 0x%x\n", code);
	} else {
		printf("Makecode: 0x%x\n", code);
	}

	if (code == EXIT_BREAK_CODE) {
		return 1;
	}

	return 0;

}

int kbd_test_scan(unsigned short ass) {
	int ipc_status;
	message msg;
	int request;
	int irq_set;

	int stop = 0;

	irq_set = subscribe_kbd();

	if (!ass) {
		while (1) {
			printf("TEST\n");

			request = driver_receive(ANY, &msg, &ipc_status);

			if (request != 0) {
				printf("driver_receive failed with: %d", request);
				continue;
			}

			if (is_ipc_notify(ipc_status)) {
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.NOTIFY_ARG & irq_set) {
						printf("\nInterrupcao efectuada com sucesso");
					}
					break;
				default:
					break;
				}
			} else {
			}
		}

		printf("ac\n");
		unsubscribe_kbd();
		printf("ad\n");
		return 0;
	}
	return 1;
}
int kbd_test_leds(unsigned short n, unsigned short *leds) {
	/* To be completed */
}
int kbd_test_timed_scan(unsigned short n) {
	/* To be completed */
}
