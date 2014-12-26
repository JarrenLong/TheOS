/* 
    This is the exeptions file for the MadOS kernel, it takes care of exeption handling in c
    Copyright (C) 2003  David Karn
    Parts of this file were taken from the FritzOS c++ kernel, which is also GPL,
    Fritzos is:
    Copyright (C) 2002 Tom Fritz

    This file is a part of MadOS

    MadOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    MadOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    For more info, look at the COPYING.txt file.
*/
#ifndef EXEPT_H
#define EXEPT_H
#include "main.h"
void interrupt_0()
{
   error("Can't Divide By 0", 4);
};
void interrupt_1()
{
   error("Debug Exeption", 4);
};
void interrupt_3()
{
   error("Breakpoint", 4);
};
void interrupt_4()
{
   error("Overflow", 4);
};
void interrupt_5()
{
   error("Bounds Check", 4);
};
void interrupt_6()
{
   error("Invalid Opcode", 4);
};
void interrupt_7()
{
   error("Coprocessor not availible", 4);
};
void interrupt_8()
{
   error("Double Fault", 4);
};
void interrupt_9()
{
   error("Coprocessor segment overrun", 4);
};
void interrupt_10()
{
   error("Invalid TSS", 4);
};
void interrupt_11()
{
   error("Segment not present", 4);
};
void interrupt_12()
{
   error("Stack Exeption", 4);
};
void interrupt_13()
{
   error("Triple Fault", 4);
};
void interrupt_14(ULONG cr2)
{
   error("Page Fault, tried to access"+(char)cr2, 4);
};
void interrupt_16()
{
   error("Coprocessor Error", 4);
};
void intelisr()
{
   error("Intel Reserved Interupt", 4);
};
#endif
