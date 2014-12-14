#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "mouse.h"
#include "bitmap.h"

unsigned int hookID;
Mouse* mouse = NULL;
Mouse lastState;


void newMouse(){
	mouse = (Mouse*) malloc(sizeof(Mouse));

	mouse->x=0;
	mouse->y=0;

	mouse->readingByte = 1;

	mouse->leftButtonDown = 0;
	mouse->middleButtonDown = 0;
	mouse->rightButtonDown = 0;

	mouse->leftButtonReleased = 0;
	mouse->middleButtonReleased = 0;
	mouse->rightButtonReleased = 0;

	mouse->cursor = loadBitmap("/home/proj/res/cursor.bmp");

	mouse->update = 0;
	mouse->draw = 1;

}
void updateMouse(){
	unsigned long byte;
	switch(mouse->readingByte){
	case 1:
		byte = mouse_read();
		if(byte & BIT(3)){
			mouse->packet[0] = byte;
			mouse->readingByte = 2;
		}
		break;
	case 2:
		mouse->packet[1] = mouse_read();
		mouse->readingByte = 3;
		break;
	case 3:
		mouse->packet[2] = mouse_read();
		mouse->readingByte = 1;
		mouse->update = 1;
		break;
	default:
		break;
	}

	if(mouse->update){
		lastState = *mouse;
		short x = (X_SIGN(mouse->packet[0]) ? (mouse->packet[1] | (0xff << 8)) : mouse->packet[1]);
		short y = (Y_SIGN(mouse->packet[0]) ? (mouse->packet[2] | (0xff << 8)) : mouse->packet[2]);

		mouse->x+=x;
		mouse->y-=y;
		mouse->leftButtonDown = LB(mouse->packet[0]);
		mouse->middleButtonDown = MB(mouse->packet[0]);
		mouse->rightButtonDown = RB(mouse->packet[0]);
		if(lastState.leftButtonDown){
			if(!mouse->leftButtonDown){
				mouse->leftButtonReleased = 1;
			}
		}
		else{
			mouse->leftButtonReleased = 0;
		}
		if(lastState.middleButtonDown){
			if(!mouse->middleButtonDown){
				mouse->middleButtonReleased = 1;
			}
		}
		else{
			mouse->middleButtonReleased = 0;
		}
		if(lastState.rightButtonDown){
			if(!mouse->rightButtonDown){
				mouse->rightButtonReleased = 1;
			}
		}
		else{
			mouse->rightButtonReleased = 0;
		}

		mouse->update = 0;
	}
}

void drawMouse(){
	if (mouse->cursor == NULL)
		return;

	int width = mouse->cursor->bitmapInfoHeader.width;
	int drawWidth = width;
	int height = mouse->cursor->bitmapInfoHeader.height;

	if (mouse->x + width < 0 || mouse->x > get_h_res() || mouse->y + height < 0
			|| mouse->y > get_v_res())
		return;

	int xCorrection = 0;
	if (mouse->x < 0) {
		xCorrection = -mouse->x;
		drawWidth -= xCorrection;
		mouse->x = 0;

		if (drawWidth > get_h_res())
			drawWidth = get_h_res();
	} else if (mouse->x + drawWidth >= get_h_res()) {
		drawWidth = get_h_res() - mouse->x;
	}

	char* bufferStartPos;
	char* imgStartPos;

	int i,j;
	for (i = 0; i < height; i++) {
		int pos = mouse->y + height - 1 - i;

		if (pos < 0 || pos >= get_v_res())
			continue;

		bufferStartPos = (char*)getMouseBuffer();
		bufferStartPos += mouse->x * 2 + pos * get_h_res() * 2;

		imgStartPos = mouse->cursor->bitmapData + xCorrection * 2 + i * width * 2;

		for(j=0; j < width; j++){
			if((*imgStartPos | ((*(imgStartPos+1))>>4)) != 0){
				*bufferStartPos = *imgStartPos;
				*(bufferStartPos+1) = *(imgStartPos+1);
			}
			bufferStartPos++;
			bufferStartPos++;
			imgStartPos++;
			imgStartPos++;
		}
	}
	mouse->draw = 0;
}
void deleteMouse(){
	free(mouse);
}
Mouse* getMouse(){
	return mouse;
}


int mouse_read(){
	unsigned long data,stat;
	sys_inb(STAT_REG, &stat);
	if((stat & OBF != 0) && (stat & AUX != 0)){
		if (sys_inb(DATA_PORT, &data) == OK)
			return data;
		else
			return -1;
	}
	return 0;
}

int mouse_write_byte(unsigned char cmd){
	unsigned long status;
	int tries=0;
	unsigned long data,stat;
	sys_inb(STAT_REG, &stat);
	if(!(IBF & stat)){
		do {
			if (sys_outb(CMD_PORT, WRITE_BYTE_MOUSE) != OK)	return 1;
			if (sys_outb(DATA_PORT, cmd) != OK)	return 1;
			status = sys_inb(DATA_PORT, &data);
			if(status == ACK){
				return 0;
			}
			tries++;
		}
		while(tries<3);
	}
	return 1;

}


int subscribe_mouse(void) {
	hookID = 12;
	unsigned long bitmask = BIT(hookID);

	mouse_write_byte(DISABLE_DATA_PACKETS);

	if (sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookID) != OK || sys_irqenable(&hookID) != OK) {
		return -1;
	}
	mouse_write_byte(DATA_PACKETS);
	mouse_write_byte(STREAM_MODE);
	return bitmask;
}

int unsubscribe_mouse() {
	mouse_write_byte(DISABLE_DATA_PACKETS);
	return sys_irqdisable(&hookID) != OK || sys_irqrmpolicy(&hookID) != OK;
}
