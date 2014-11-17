#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#define EXIT_BREAK_CODE 0x81 //ESC KEY BREAK CODE
#define DELAY_US 20000
#define IRQ_KBD 1
#define OUT_BUF 0x60
#define INPT_BUF 0x60
#define STAT_REG 0x64
#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)
#define OBF BIT(0)
#define BIT(n) (0x01<<(n))

int subscribe_kbd(void);

int unsubscribe_kbd();

int kbc_read();
#endif /* __KEYBOARD_H */
