#include <minix/drivers.h>

int main()
{
	/* Initialize service */

	sef_startup();

	test_init(0x105, 5);

	return 0;
}
