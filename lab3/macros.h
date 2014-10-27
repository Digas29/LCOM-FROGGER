#ifndef _MACROS_
#define _MACROS_




#define BIT(n) (0x01<<(n))

#define EXIT_BREAK_CODE 0x81 //ESC KEY BREAK CODE

#define DELAY_US 20000

#define IRQ_KBD 1

#define TIMER0_IRQ 0

#define OUT_BUF 0x60

#define INPT_BUF 0x60

#define STAT_REG 0x64

#define KBC_CMD_REG 0x64

#define PAR_ERR BIT(7)

#define TO_ERR BIT(6)

#define OBF BIT(0)

#define IBF BIT(1)

#define SPECIAL_KEY 0xe0 // FIRST BYTE OF A 2 BYTES SCANCODE

#define LEDS 0xed //Switch on/off KBD LEDs

#define RESEND_LEDS 0xfe

#define ERROR_LEDS 0xfc

#endif
