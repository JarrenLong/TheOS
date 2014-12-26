/* 
    This is the common functions library for the MadOS kernel, it contains the functions used often
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


#ifndef COMMON_H
#define COMMON_H
#include "main.h"
void mwait(int time);
int wait;
int tempk;

void mwait(int time)
{
	wait=0;
    while (wait<time*10000000) {wait++;}

}

void reboot()
{


        while ( tempk & 2 )
        {
                tempk = inb( 0x64 );

                if ( tempk & 1 )
                        inb( 0x60 );
        }


        outb(0x64, 0xFE);

}

void *memcpy(void *dst_ptr, const void *src_ptr, unsigned count)
{
	void *ret_val = dst_ptr;
	const char *src = (const char *)src_ptr;
	char *dst = (char *)dst_ptr;
	for(; count != 0; count--)
		*dst++ = *src++;
	return ret_val;
}

unsigned strlen(char* string)
{
	unsigned i;

	for (i = 0; *string != '\0';string++)
    {
		i++;
     }

	return i;
}

void* memsetw(void *dest, int fill, unsigned much)
{
	unsigned short *temp = (unsigned short *) dest;

	for (;much > 0;much--)
     {
		*temp++=fill;
     }

	return dest;
}
char *addtostring(char *a,char *b)
{
int loop1=0;
int loop2=0;
while( a[loop1]!='\0')
{
loop1++;
}
while( b[loop2]!='\0')
{
a[loop1]=b[loop2];
loop1++;
loop2++;
}
return a;
}

int strcmp(char* string, char* string2)
{
	int i = 0;
    if (strlen(string) == strlen(string2))
    {
	while (*string != '\0' && *string2 != '\0')
    {
        if (*string == *string2)
        {
        *string++;
        *string2++;
		i++;
        }
        else
        {
        return 0;
        }
     }
     }
     else {
     return 0;
     }
	return 1;
}
#endif

