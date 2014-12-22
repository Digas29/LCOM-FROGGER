#include <stdio.h>
#include <assert.h>
#include <minix/drivers.h>
#include "frogger.h"
#include "graphics.h"
#include "mouse.h"


int main(int argc, char **argv) {
	sef_startup();
	vg_init(0x114);


	Frogger* frogger = newFrogger();


	while(frogger->complete != 1){
		updateFrogger(frogger);
		if(frogger->draw){
			drawFrogger(frogger);
		}
	}
	deleteFrogger(frogger);

	vg_exit();
    return 0;
}
