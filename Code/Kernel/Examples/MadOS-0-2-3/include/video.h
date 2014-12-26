/* 
    This is the video file for the MadOS kernel, it takes care of all video functions used by the MadOS kernel
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


#ifndef VIDEO_H
#define VIDEO_H
#define WHITE_BLACK	0x07	// white on black text
#define RED_GREEN	0x26	// red on green text
#define BLUE_BLACK	1	// Blue on black
#define RED_BLACK	4	// red on black
#define PURPLE_BLACK	0x5	// purple on black
#define YELLOW_BLACK	0x6	// yellow on black
#define LIGHTBLUE_BLACK	0x9	// light blue on black
#define DARKBLUE_BLACK	0x8	// dark blue on black
#define GREEN_BLACK		10	// green on black
#define MAGENTA_BLACK		12	// magenta on black
#define BLUE_BLACK_BLINK  0x81
#define BLACK_MAG_BLINK  0x0B
int collum = 0;
unsigned int printf(char *message);
unsigned int putchar(char character, UINT line, UINT collum);
unsigned textatt = WHITE_BLACK;
unsigned int putkey(char character);
void scroll();
void inccollum();
void update_cursor();

void clearscreen() // clear the entire text screen
{
	unsigned int i=0;
 char *vidmem = (char *) 0xb8000;

	while(i < (80*25*2))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=WHITE_BLACK;
		i++;
	};
 collum = 0;
 update_cursor();
};


unsigned int printf(char *message) // the message and then the line #
{
    scroll();
	char *vidmem = (char *) 0xb8000;

	while(*message!=0)
	{

            putchar(*message, 24,collum);
            inccollum();
			*message++;
	};
	return(1);
};

unsigned int putchar(char character, UINT line, UINT collumn) // the message and then the line #
{
	char *vidmem = (char *) 0xb8000;
	UINT i=0;

	        i=(line*80+collumn)*2;
			vidmem[i]=character;
            i++;
			vidmem[i]=textatt;
	return(1);
};

void inccollum()
{
collum++;
if(collum > 79)
{
collum = 0;
scroll();
}
update_cursor();
}

void scroll()
{
char *vidmem = (char *) 0xb8000;
memcpy(vidmem, vidmem + 80*2, 80*24*2);
vidmem = (char *) 0xb8000 +(80*24*2);
unsigned int i=0;
	while(i < (160))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=WHITE_BLACK;
		i++;
	};
 collum = 0;
 update_cursor();
}

unsigned int putkey(char character) // the message and then the line #
{
	char *vidmem = (char *) 0xb8000;
	UINT i=0;

	i=(24*80+collum)*2;
    	if ( character == 0x08 ) // 0x08 == backspace
	{
        if(collum != 0)
        {
        collum--;
        vidmem[i]=' ';
	    vidmem[i + 1]=WHITE_BLACK;
        }
  	}

	else if (character == 0x09 )  // 0x09 == Tab
	{
     inccollum();
     inccollum();
     inccollum();
     inccollum();
	}
    else{
	vidmem[i]=character;
    i++;
	vidmem[i]=textatt;
    inccollum();
    }
    update_cursor();
	return(1);
};
char *getstring()
{
    char *command;
	char *vidmem = (char *) 0xb8000;
	UINT i=0;
    int index = 0;
    char character;
	i=(24*80+collum)*2;
 character = getchar();
    while (character != ENTER)
    {

        i=(24*80+collum)*2;
    	if ( character == 0x08 ) // 0x08 == backspace
	    {
        if(collum != 0)
        {
        collum--;
        vidmem[i]=' ';
        i++;
	    vidmem[i]=WHITE_BLACK;
        i++;
        index--;
        }
  	}
	else if (character == 0x09 )  // 0x09 == Tab
	{
     inccollum();
     inccollum();
     inccollum();
     inccollum();
     index += 4;
	}
    else{
	vidmem[i]=character;
    i++;
	vidmem[i]=textatt;
    inccollum();
    command[index] = character;
	index++;
    }
    update_cursor();
    character = getchar();
    }
    command[index] = '\0';
	return(command);
}
void update_cursor()
	{
		unsigned short	position=(24*80) + collum;

		outb(0x3D4, 0x0F);
		outb(0x3D5, (unsigned char)(position&0xFF));
		outb(0x3D4, 0x0E);
		outb(0x3D5, (unsigned char)((position>>8)&0xFF));
	}

void paintscreen(color_t color) // paint the entire text screen
{
	unsigned int i=0;
    char *vidmem = (char *) 0xb8000;

	while(i < (80*25*2))
	{
		i++;
		vidmem[i]=color;
		i++;
	};
};

#endif


