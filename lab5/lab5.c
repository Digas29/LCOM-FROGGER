#include <minix/drivers.h>

int main()
{
	/* Initialize service */

	sef_startup();

	//test_init(0x105, 5);
	test_square(0,0,500,50);

	return 0;
}
