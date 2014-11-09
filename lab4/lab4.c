#include "test4.h"
#include <minix/drivers.h>

int main(int argc, char *argv[]) {
	sef_startup();
	test_gesture(10,2);
	return 0;
}
