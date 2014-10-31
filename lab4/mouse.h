#ifndef __MOUSE_H
#define __MOUSE_H


#define IRQ_MOUSE 12

#define TIMER0_IRQ 0

#define BIT(n) (0x01<<(n))

#define OUT_BUF 0x60

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

#define ENABLE_DATA_PACKETS 0xf4

#define Y_OVF(n) (((n) & BIT(7)) >> 7)

#define X_OVF(n) (((n) & BIT(6)) >> 6)

#define Y_SIGN(n) (((n) & BIT(5)) >> 5)

#define X_SIGN(n) (((n) & BIT(4)) >> 4)

#define MB(n) (((n) & BIT(2)) >> 2)

#define RB(n) (((n) & BIT(1)) >> 1)

#define LB(n) ((n) & BIT(0))


#endif
