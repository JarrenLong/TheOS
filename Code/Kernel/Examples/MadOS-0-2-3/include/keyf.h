/* 
    This is the key function file for the MadOS kernel, it takes care of functions for getting keys
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
#ifndef KEYF_H
#define KEYF_H

#include "main.h"
// Create a typedef for getting keys, to hold the correct value:
typedef UCHAR KEY;

// Create a raw scancode data type for getting scancodes:
typedef UINT RAWKEY;

int shift;

UINT mgetkey();
UINT getkey();

KEY const getchar()
{
	RAWKEY scan;
	KEY retchar;

	scan = mgetkey();

	if (shift == 1)
		retchar = asciiShift[ scan ];

	else if (shift != 1)
		retchar = asciiNonSh[ scan ];

	return retchar;				
}


UINT mgetkey()
{

RAWKEY scancode;


	while (TRUE)
	{

		while ((inb( 0x64 ) & 1 ) == FALSE)
			;
		scancode = getkey();

		if (scancode & KEYPRESS)
		{

			scancode &= 0x7F;

			if (scancode == KEY_RLEFT_SHIFT || scancode == KEY_RRIGHT_SHIFT)
				shift = 0;

			continue;
		}

		if (scancode == KEY_RLEFT_SHIFT || scancode == KEY_RRIGHT_SHIFT)
		{
			shift = 1;

			continue;
		}

		return scancode;
	}
}

UINT getkey()
{
	int rawkey;

	rawkey = inb(0x60);

	return rawkey;
}
#endif
