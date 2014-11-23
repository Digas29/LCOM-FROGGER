#include <minix/drivers.h>
#include "pixmap.h"

int main(int argc, char **argv) {

	/* Initialize service */

	sef_startup();

    //test_init(0x150, 5);
    //test_square(200,300, 100,50);
    //test_line(500,200,50,200,50);
    //test_xpm(50,50,penguin);
    test_move(0,0,cross,0,800,20);
    //test_controller();

}
