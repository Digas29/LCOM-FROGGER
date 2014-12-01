#pragma once
#include "bitmap.h"

#define IRQ_MOUSE 12
#define BIT(n) (0x01<<(n))
#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)
#define OBF BIT(0)
#define IBF BIT(1)
#define STAT_REG 0x64
#define WAIT_TICKS micros_to_ticks(20000)
#define WRITE_BYTE_MOUSE 0xd4
#define DATA_PORT 0x60
#define CMD_PORT 0x64
#define ACK 0xfa
#define STREAM_MODE 0xea
#define DATA_PACKETS 0xf4
#define DISABLE_DATA_PACKETS 0xf5
#define Y_OVF(n) (((n) >> 7) & BIT(0))
#define X_OVF(n) (((n) >> 6) & BIT(0))
#define Y_SIGN(n) (((n) >> 5) & BIT(0))
#define X_SIGN(n) (((n) >> 4) & BIT(0))
#define MB(n) (((n) & BIT(2)) >> 2)
#define RB(n) (((n) & BIT(1)) >> 1)
#define LB(n) ((n) & BIT(0))
#define AUX BIT(5)

typedef struct {
    int x, y;
    int readingByte;
    unsigned long packet[3];

    int leftButtonDown;
    int middleButtonDown;
    int rightButtonDown;

    int leftButtonReleased;
    int rightButtonReleased;
    int middleButtonReleased;

    Bitmap * cursor;

    int update;
    int draw;
} Mouse;


void newMouse();
void updateMouse();
void drawMouse();
void deleteMouse();
Mouse* getMouse();

int mouse_read();

int mouse_write_byte(unsigned char cmd);

int subscribe_mouse(void);

int unsubscribe_mouse();

void packet_sum();
