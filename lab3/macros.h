#ifndef _MACROS_
#define _MACROS_

/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

/**< @brief clock frequency for timer in PC and AT */

#define BIT(n) (0x01<<(n))
#define EXIT_BREAK_CODE 0x81
#define DELAY_US 20000
#define IRQ_KBD 1
#define OUT_BUF 0x60
#define INPT_BUF 0x06
#define STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)

#endif
