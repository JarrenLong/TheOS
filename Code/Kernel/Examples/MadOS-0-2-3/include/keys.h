/* 
    This is the keys define file for the MadOS kernel, it defines the keys used by the MadOS kernel
    Copyright (C) 2003  David Karn
    Parts of this file were taken from the FritzOS c++ kernel, which is also GPL,
    Fritzos is
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


#ifndef KEYS_H
#define KEYS_H


#define ESC			27

#define BACKSPACE '\b'

#define TAB '\t'

#define ENTER '\n'

#define KEY_ESC			1
#define	KEY_F1		0x80
#define	KEY_F2		(KEY_F1 + 1)
#define	KEY_F3		(KEY_F2 + 1)
#define	KEY_F4		(KEY_F3 + 1)
#define	KEY_F5		(KEY_F4 + 1)
#define	KEY_F6		(KEY_F5 + 1)
#define	KEY_F7		(KEY_F6 + 1)
#define	KEY_F8		(KEY_F7 + 1)
#define	KEY_F9		(KEY_F8 + 1)
#define	KEY_F10		(KEY_F9 + 1)
#define	KEY_F11		(KEY_F10 + 1)
#define	KEY_F12		(KEY_F11 + 1)


#define	KEY_INS		0x90
#define	KEY_DEL		(KEY_INS + 1)
#define	KEY_HOME		(KEY_DEL + 1)
#define	KEY_END		(KEY_HOME + 1)
#define	KEY_PGUP		(KEY_END + 1)
#define	KEY_PGDN		(KEY_PGUP + 1)
#define	KEY_LEFT		(KEY_PGDN + 1)
#define	KEY_UP		(KEY_LEFT + 1)
#define	KEY_DOWN		(KEY_UP + 1)
#define	KEY_RIGHT		(KEY_DOWN + 1)


#define	KEY_META_ALT	0x0200
#define	KEY_META_CTRL	0x0400
#define	KEY_META_SHIFT	0x0800
#define	KEY_META_ANY	(KEY_META_ALT | KEY_META_CTRL | KEY_META_SHIFT)
#define	KEY_META_CAPS	0x1000
#define	KEY_META_NUM	0x2000
#define	KEY_META_SCRL	0x4000


#define	KEY_PRNT	( KEY_RIGHT + 1 )
#define	KEY_PAUSE	( KEY_PRNT + 1 )
#define	KEY_LWIN	( KEY_PAUSE + 1 )
#define	KEY_RWIN	( KEY_LWIN + 1 )
#define	KEY_MENU	( KEY_RWIN + 1 )

#define	KEY_RLEFT_CTRL		0x1D
#define	KEY_RLEFT_SHIFT		0x2A
#define	KEY_RCAPS_LOCK		0x3A
#define	KEY_RLEFT_ALT		0x38
#define	KEY_RRIGHT_ALT		0x38
#define	KEY_RRIGHT_CTRL		0x1D
#define	KEY_RRIGHT_SHIFT		0x36
#define	KEY_RSCROLL_LOCK		0x46
#define	KEY_RNUM_LOCK		0x45
#define	KEY_RDEL			0x53

#define KEYPRESS 0x80

#define MAXKEYBUFFER 64

static const unsigned char asciiNonSh[] = {' ', ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9',
'0', '-', '=', BACKSPACE, TAB, 'q', 'w',   'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',   '[', ']', ENTER, 0,
'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
'm', ',', '.', '/', 0, 0, 0, ' ',   0, KEY_F1,   KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0,
KEY_HOME, KEY_UP, KEY_PGUP,'-', KEY_LEFT, '5',   KEY_RIGHT, '+', KEY_END, KEY_DOWN, KEY_PGDN, KEY_INS, KEY_DEL, 0, 0, 0, KEY_F11, KEY_F12};

static const unsigned char asciiShift[] = {' ', ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(',
')', '_', '+', BACKSPACE, TAB, 'Q', 'W',   'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',   '{', '}', ENTER, 0,
'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N',
'M', '<', '>', '?', 0, 0, 0, ' ',   0, KEY_F1,   KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0,
KEY_HOME, KEY_UP, KEY_PGUP, '-', KEY_LEFT, '5',   KEY_RIGHT, '+', KEY_END, KEY_DOWN, KEY_PGDN, KEY_INS, KEY_DEL, 0, 0, 0, KEY_F11, KEY_F12};

#endif
