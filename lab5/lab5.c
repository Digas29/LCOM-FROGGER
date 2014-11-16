#include <minix/drivers.h>
#include "pixmap.h"

int main()
{
	/* Initialize service */

	sef_startup();

	test_move(100, 400, penguin, 0, 200, 10);

	return 0;
}
