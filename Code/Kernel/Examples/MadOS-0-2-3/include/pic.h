/* 
    This is the PIC file for the MadOS kernel, it takes care of remapping the PIC
    Copyright (C) 2003  David Karn

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
#ifndef PIC_H
#define PIC_H
#include "main.h"
#define	PIC1		0x20
#define	PIC2		0xA0
#define	PIC1_COMMAND	PIC1
#define	PIC1_DATA	(PIC1+1)
#define	PIC2_COMMAND	PIC2
#define	PIC2_DATA	(PIC2+1)
#define	PIC_EOI		0x20
#define	ICW1_ICW4	0x01
#define	ICW1_SINGLE	0x02
#define	ICW1_INTERVAL4	0x04
#define	ICW1_LEVEL	0x08
#define	ICW1_INIT	0x10
#define	ICW4_8086	0x01
#define	ICW4_AUTO	0x02
#define	ICW4_BUF_SLAVE	0x08
#define	ICW4_BUF_MASTER	0x0C
#define	ICW4_SFNM	0x10

void remap_pics(int pic1, int pic2)
{
	UCHAR	a1, a2;
	a1=inb(PIC1_DATA);

	a2=inb(PIC2_DATA);
	outb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);

	outb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
	outb(PIC1_DATA, pic1);
	outb(PIC2_DATA, pic2);
	outb(PIC1_DATA, 4);
	outb(PIC2_DATA, 2);
	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);
	outb(PIC1_DATA, a1);

	outb(PIC2_DATA, a2);
}


void NMI_enable(void)
{
	outb(0x70, inb(0x70)&0x7F);
}

void NMI_disable(void)
{
	outb(0x70, inb(0x70)||0x80);
}


#endif
