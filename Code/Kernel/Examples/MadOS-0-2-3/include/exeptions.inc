;    This is the exeptions file for the MadOS kernel, it takes care of handling exeptions in Nasm Assembly
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
isr0:
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

isr1:
 pusha
 push gs
 push fs
 push ds
 push es

 extern _interrupt_1
 call _interrupt_1

 pop es
 pop ds
 pop fs
 pop gs
 popa
 iret

isr3:
 pusha
 push gs
 push fs
 push ds
 push es

 extern _interrupt_3
 call _interrupt_3

 pop es
 pop ds
 pop fs
 pop gs
 popa
 iret

isr4:
 pusha
 push gs
 push fs
 push ds
 push es

 extern _interrupt_4
 call _interrupt_4

 pop es
 pop ds
 pop fs
 pop gs
 popa
 iret

isr5:
 pusha
 push gs
 push fs
 push ds
 push es

 extern _interrupt_5
 call _interrupt_5

 pop es
 pop ds
 pop fs
 pop gs
 popa
 iret


isr6:
 pusha
 push gs
 push fs
 push ds
 push es

 extern _interrupt_6
 call _interrupt_6

 pop es
 pop ds
 pop fs
 pop gs
 popa
 iret


isr7:
 pusha
 push gs
 push fs
 push ds
 push es

 extern _interrupt_7
 call _interrupt_7

 pop es
 pop ds
 pop fs
 pop gs
 popa
 iret


isr8:
 pusha
 push gs
 push fs
 push ds
 push es

 extern _interrupt_8
 call _interrupt_8

 pop es
 pop ds
 pop fs
 pop gs
 popa
 iret


isr9:
 pusha
 push gs
 push fs
 push ds
 push es

 extern _interrupt_9
 call _interrupt_9

 pop es
 pop ds
 pop fs
 pop gs
 popa
 iret


isr10:
 pusha
 push gs
 push fs
 push ds
 push es

 extern _interrupt_10
 call _interrupt_10

 pop es
 pop ds
 pop fs
 pop gs
 popa
 iret

isr12:
 pusha
 push gs
 push fs
 push ds
 push es

 extern _interrupt_12
 call _interrupt_12

 pop es
 pop ds
 pop fs
 pop gs
 popa
 iret

isr13:
 pusha
 push gs
 push fs
 push ds
 push es

 extern _interrupt_13
 call _interrupt_13

 pop es
 pop ds
 pop fs
 pop gs
 popa
 iret

isr14:
 pusha
 push gs
 push fs
 push ds
 push es

 mov eax, cr2 ; CR2 contians the address that the program tried to access
 push eax ; now our C function can get it

 extern _interrupt_14
 call _interrupt_14

 pop eax ; have to pop what we pushed or the stack gets messed up

 pop es
 pop ds
 pop fs
 pop gs
 popa
 iret

isr16:
 pusha
 push gs
 push fs
 push ds
 push es

 extern _interrupt_16
 call _interrupt_16

 pop es
 pop ds
 pop fs
 pop gs
 popa
 iret
