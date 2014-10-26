.include "test3.h"
.include "macros.h"
.include "kbc.h"

XDEF _kbc_read
XDEF _printf

.text

LC0:
	.ascii "Brakecode: 0x%x\12\0"
LC1:
	.ascii "Makecode: 0x%x\12\0"
LC2:
	.ascii "Brakecode: 0xe0%x\12\0"
LC3:
	.ascii "Makecode: 0xe0%x\12\0"

.globl _kbd_handler_asm
_kbd_handler_asm:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	call	_kbc_read
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	cmpl	$224, %eax
	je	L26
	cmpl	$-1, %eax
	je	L27
	cmpl	$129, %eax
	je	L25
	jmp	L24
L27:
	movl	$1, %eax
	jmp	L28
L25:
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$LC0, (%esp)
	call	_printf
	movl	$1, %eax
	jmp	L28
L26:
	movl	$1, _special
L24:
	movl	_special, %eax
	cmpl	$1, %eax
	je	L31
	cmpl	$1, %eax
	jb	L30
	cmpl	$2, %eax
	je	L32
	jmp	L29
L30:
	movl	-12(%ebp), %eax
	andl	$128, %eax
	testl	%eax, %eax
	je	L33
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$LC0, (%esp)
	call	_printf
	jmp	L29
L33:
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$LC1, (%esp)
	call	_printf
	movl	$0, _counter
	jmp	L29
L31:
	movl	_special, %eax
	addl	$1, %eax
	movl	%eax, _special
	jmp	L29
L32:
	movl	-12(%ebp), %eax
	andl	$128, %eax
	testl	%eax, %eax
	je	L35
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$LC2, (%esp)
	call	_printf
	movl	$0, _special
	jmp	L29
L35:
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$LC3, (%esp)
	call	_printf
	movl	$0, _special
	movl	$0, _counter
L29:
	movl	$0, %eax
L28:
	leave
	ret
