#include <minix/drivers.h>

int main()
{
	/* Initialize service */

	sef_startup();

	test_square(10, 10, 10, 10);

	return 0;
}
