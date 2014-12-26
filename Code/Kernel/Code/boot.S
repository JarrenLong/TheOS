#include "multiboot.h"

#ifdef HAVE_ASM_USCORE
# define EXT_C(sym)			_ ## sym
#else
# define EXT_C(sym)			sym
#endif

#define STACK_SIZE			0x4000

#ifdef __ELF__
# define MULTIBOOT_HEADER_FLAGS		0x00000003
#else
# define MULTIBOOT_HEADER_FLAGS		0x00010003
#endif

	.text

	.globl	start, _start

start:
_start:
	jmp	multiboot_entry
	.align	4
multiboot_header:
	.long	MULTIBOOT_HEADER_MAGIC
	.long	MULTIBOOT_HEADER_FLAGS
	.long	-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
#ifndef __ELF__
	.long	multiboot_header
	.long	_start
	.long	_edata
	.long	_end
	.long	multiboot_entry
#endif

multiboot_entry:
	movl	$(stack + STACK_SIZE), %esp
	pushl	$0
	popf
	pushl	%ebx
	pushl	%eax
	call	EXT_C(cmain)
	pushl	$halt_message
	call	EXT_C(printf)

loop:	hlt
	jmp	loop

halt_message:
	.asciz	"Halted."
	.comm	stack, STACK_SIZE
