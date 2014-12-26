;    This is the Loader file for the MadOS kernel, it takes care of loading the kernel, and assembly functions
;    Copyright (C) 2003  David Karn
;    Parts of this file were taken from the FritzOS c++ kernel, which is also GPL,
;    Fritzos is:
;    Copyright (C) 2002 Tom Fritz
;
;    This file is a part of MadOS
;
;    MadOS is free software; you can redistribute it and/or modify
;    it under the terms of the GNU General Public License as published by
;    the Free Software Foundation; either version 2 of the License, or
;    (at your option) any later version.
;
;    MadOS is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU General Public License for more details.
;
;    You should have received a copy of the GNU General Public License
;    along with this program; if not, write to the Free Software
;    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
;
;    For more info, look at the COPYING.txt file.

%include "grub.inc" ; needed for the multiboot header

[BITS 32]
[section .text]
                 add [gdt_pointer + 2], eax
                 lgdt [gdt_pointer]

[global start]
[extern _k_main] ; this is in the c file



start:

  call _k_main

  cli  ; stop interrupts
  hlt ; halt the CPU




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Multiboot header for GRUB bootloader. This must be in the first 8K
; of the kernel file. We use the aout kludge so it works with ELF,
; DJGPP COFF, Win32 PE, or other formats.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; these are in the linker script file
EXTERN code, bss, end

ALIGN 4
mboot:
        dd MULTIBOOT_HEADER_MAGIC
        dd MULTIBOOT_HEADER_FLAGS
        dd MULTIBOOT_CHECKSUM
; aout kludge. These must be PHYSICAL addresses
        dd mboot
        dd code
        dd bss
        dd end
        dd start

GLOBAL _isr0
GLOBAL _isr1
GLOBAL _isr3
GLOBAL _isr4
GLOBAL _isr5
GLOBAL _isr6
GLOBAL _isr7
GLOBAL _isr8
GLOBAL _isr9
GLOBAL _isr10
GLOBAL _isr11
GLOBAL _isr12
GLOBAL _isr13
GLOBAL _isr14
GLOBAL _isr16
GLOBAL _isr17
GLOBAL _isr18
GLOBAL _isr19
GLOBAL _int0
GLOBAL _int1
GLOBAL _n
GLOBAL _lowisr
GLOBAL _isr31
GLOBAL _intelreserved
GLOBAL _loadidt

_isr0:
cli
hlt
iret
_isr1:
cli
hlt
iret
_isr2:
cli
hlt
iret
_isr3:
cli
hlt
iret
_isr4:
cli
hlt
iret
_isr5:
cli
hlt
iret
_isr6:
cli
hlt
iret
_isr7:
cli
hlt
iret
_isr8:
cli
hlt
iret
_isr9:
cli
hlt
iret
_isr10:
cli
hlt
iret
_isr11:
cli
hlt
iret
_isr12:
cli
hlt
iret
_isr13:
cli
hlt
iret
_isr14:
cli
hlt
iret
_isr15:
cli
hlt
iret
_isr16:
cli
hlt
iret
_isr17:
cli
hlt
iret
_isr18:
cli
hlt
iret
_isr19:
cli
hlt
iret
_int0:
pusha
push gs
push fs
push ds
push es

extern _interrupt_0
call _interrupt_0

pop es
pop ds
pop fs
pop gs
popa
iret

_int1:
pusha
push gs
push fs
push ds
push es

extern _interrupt_0
call _interrupt_0

pop es
pop ds
pop fs
pop gs
popa
iret
_isr31:
cli
hlt
iret
_lowisr:
        PUSHA                ; Save Registers
        MOV AL, 0x20        ; Send EOI ( end of interrupt )
        OUT 0x20, al
        POPA                ; Restore changed registers
        IRET                ; (I)nterrupt (RET)urn
_intelreserved:
cli
hlt
iret
_n:
cli
hlt
iret


gdt:
    ; NULL descriptor
    dw 0              ; limit
    dw 0              ; base
    db 0              ; base
    db 0              ; type
    db 0              ; limit, flags
    db 0              ; base

    SupCodeSel   EQU $-gdt
    dw 0xFFFF
    dw 0
    db 0
    db 0x9a
    db 0xCF
    db 0

    SupDataSel   EQU $-gdt
    dw 0xFFFF
    dw 0
    db 0
    db 0x92
    db 0xCF
    db 0

    UserCodeSel   EQU $-gdt
    dw 0xFFFF
    dw 0
    db 0
    db 0xfa
    db 0xCF
    db 0

    UserDataSel   EQU $-gdt
    dw 0xFFFF
    dw 0
    db 0
    db 0xf2
    db 0xCF
    db 0

gdt_end:

gdt_pointer:
  dw gdt_end - gdt - 1
  dd gdt
  


[global _read_cr0]
_read_cr0:
        mov eax, cr0
        retn

[global _write_cr0]
_write_cr0:
        push ebp
        mov ebp, esp
        mov eax, [ebp+8]
        mov cr0,  eax
        pop ebp
        retn

[global _read_cr3]
_read_cr3:
        mov eax, cr3
        retn

[global _write_cr3]
_write_cr3:
        push ebp
        mov ebp, esp
        mov eax, [ebp+8]
        mov cr3, eax
        pop ebp
        retn

_loadidt:
                push ebp
                mov ebp,esp
                lidt [ebp+8]
                mov esp,ebp
                pop ebp

                ret

