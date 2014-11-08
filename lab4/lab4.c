#include "test4.h"
#include <minix/drivers.h>

int main(int argc, char *argv[]) {
	sef_startup();
	test_packet(5);
	return 0;
}
