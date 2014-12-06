#include <stdio.h>
#include <minix/drivers.h>
#include "frogger.h"
#include "graphics.h"
#include "mouse.h"


int main(int argc, char **argv) {
	sef_startup();
	vg_init(0x114);


	Frogger* frogger = newFrogger();

	drawFrogger(frogger);

	while(frogger->complete != 1){
		updateFrogger(frogger);

		if(frogger->complete != 1){
			flipMouseBuffer();
	 		if(getMouse()->draw){
				drawMouse();
			}
	 		flipVRAM();
		}
	}

	deleteFrogger(frogger);

	vg_exit();
    return 0;
}
