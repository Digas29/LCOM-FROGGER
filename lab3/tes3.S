#include "macros.h"

.globl _scanCode
.globl _kbd_handler_asm

.text

_kbd_handler_asm:
	inb $OUT_BUF, %al
	movb %al, _scanCode
	ret
