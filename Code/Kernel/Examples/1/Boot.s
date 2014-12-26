STACKSIZE			equ 0xFFFF
MBOOT_PAGE_ALIGN    equ 1 << 0												; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1 << 1												; Provide the kernel with memory info
MBOOT_GRAPHICS		equ 1 << 2
																			; You could add more flags here, and GrUB might give it you (if it's compiled right)
MBOOT_HEADER_MAGIC  equ 0x1BADB002											; Magic value defined by the multiboot specification
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_GRAPHICS	; If you add flags, don't forget to add them here
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)			; The magic value, flags and checksum should equal zero

[BITS 32]							; All instructions should be 32-bit.

[GLOBAL Multiboot]					; Make 'Multiboot' accessible from C++.
[GLOBAL Start]						; Kernel entry point.

[EXTERN code]						; Start of the '.text' section. This is where the actual code resides
[EXTERN bss]						; Start of the '.bss' section. This section is zeroed out by our multiboot-compliant bootloader
[EXTERN end]						; Last memory address occupied by the kernel
[EXTERN Main]						; This is the C++ entry point, which is invoked after the global constructor list has been invoked
[EXTERN LoadConstructors]

Multiboot:
  dd  MBOOT_HEADER_MAGIC			; The bootloader will search for this value on each page boundary in the kernel file
  dd  MBOOT_HEADER_FLAGS			; This specifies the extra information passed by the bootloader
  dd  MBOOT_CHECKSUM
   
  dd  Multiboot						; The Multiboot header needs to be at the start of the file
  dd  code							; Start of kernel '.text' (code) section
  dd  bss							; Start of kernel '.data' section
  dd  end							; End of kernel
  dd  Start							; This is the entry point of the kernel


Start:
	mov esp, StackEnd				; Set up the stack. The stack grows upwards, so we pass it the end rather than the beginning
	mov ebp, StackEnd
	cli								; Disable interrupts, to stop interrupts being processed when there is no IDT set up
	call LoadConstructors			; Load global constructors
	push ebx						; Pass a pointer to the multiboot header, filled in by the bootloader
	push esp						; Give Main the address of the stack
	call Main						; Jump to the C++ entry point
	jmp $							; Enter an infinite loop, to stop the processor
									; executing whatever rubbish is in the memory
									; after the kernel

section .bss						; What better place to put the stack then the one area of the program which is certain to be zeroed out?
StackStart:
	resb STACKSIZE					; Reserve 0xFFFF bytes (64k) of memory for the stack
StackEnd:							; The stack grows upwards, so a good stack size is rather important unless you happen to like overwriting your kernel's code