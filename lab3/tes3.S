.file	"test3.c"
.lcomm _special,16

.text

Break:
	.ascii "Brakecode: 0x%x\12\0"
Make:
	.ascii "Makecode: 0x%x\12\0"
specialBreak:
	.ascii "Brakecode: 0xe0%x\12\0"
specialMake:
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
	je	special
	cmpl	$-1, %eax
	je	unsuccess
	cmpl	$129, %eax
	je	exitCode
	jmp	continue
unsuccess:
	movl	$1, %eax
	jmp	fim
exitCode:
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$Break, (%esp)
	call	_printf
	movl	$1, %eax
	jmp	fim
special:
	movl	$1, _special
continue:
	movl	_special, %eax
	cmpl	$1, %eax
	je	specialHandler
	cmpl	$1, %eax
	jb	print
	cmpl	$2, %eax
	je	specialPrint
	jmp	ret0
print:
	movl	-12(%ebp), %eax
	andl	$128, %eax
	testl	%eax, %eax
	je	makeCode
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$Break, (%esp)
	call	_printf
	jmp	ret0
makeCode:
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$Make, (%esp)
	call	_printf
	jmp	ret0
specialHandler:
	movl	_special, %eax
	addl	$1, %eax
	movl	%eax, _special
	jmp	ret0
specialPrint:
	movl	-12(%ebp), %eax
	andl	$128, %eax
	testl	%eax, %eax
	je	specialMakeCode
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$specialBreak, (%esp)
	call	_printf
	movl	$0, _special
	jmp	ret0
specialMakeCode:
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$specialMake, (%esp)
	call	_printf
	movl	$0, _special
ret0:
	movl	$0, %eax
fim:
	leave
	ret
