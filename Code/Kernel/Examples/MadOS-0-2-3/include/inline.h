/* 
    This is the inline assembly library for the MadOS kernel, it takes care of Inline functions
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
*/

#ifndef INLINE_H
#define INLINE_H

inline unsigned inb(unsigned port);

inline void outb(unsigned port, unsigned val);

inline void sti();

inline void cli();

int intsenabled;



inline unsigned inb(unsigned port)
{
	unsigned inpv;

	asm volatile("inb %w1,%b0"
		: "=a"(inpv)
		: "d"(port));

	return inpv;
}


inline void outb(unsigned port, unsigned vto)
{
 asm volatile("outb %b0,%w1"
		:
		: "a"(vto), "d"(port));
}

inline void enable_ints()
{
	asm volatile( "sti" );
	intsenabled = 1;
}

inline void disable_ints()
{
	asm volatile( "cli" );
	intsenabled = 0;
}


#endif
