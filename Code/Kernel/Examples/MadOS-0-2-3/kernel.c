/* 
    This is the main file of the MadOS kernel, it takes care of most of the stuff going on
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
#ifndef KERNEL_C
#define KERNEL_C
#include "include/main.h"
int boot();
int mainloop();
k_main() // like main in a normal C program
  {
  boot();
  printf("Hi, welcome to MadOS");
  textatt = RED_BLACK;
  printf("Testing color");
  textatt = BLUE_BLACK_BLINK;
  printf("testing blinking text");
  textatt = WHITE_BLACK;
  printf("Pausing");
  mwait(50);
  printf("Clearing Screen");
  clearscreen();
   printf("Type whatever you want,");
   printf("Testing scrolling");
  printf("Press shift+escape after screen scrolls to reboot");
  mwait(30);
  scroll();
  mainloop();
 };

 int boot()
 {
 clearscreen();
 textatt = WHITE_BLACK;
 printf("Booting MadOS");
 printf("Remapping PIC's");
 remap_pics(0x20, 0x28);
 printf("Remapp Sucessfull ");
 printf(" ");
 printf("Setting up idt");
 setupidt();
 printf("IDT setup complete");
 printf(" ");
 printf("Loading idt");
 loadidt(&idt);
 printf("IDT loaded");
 printf(" ");
 printf("Not Enabling Interupts (thnx to xsisms smartness)");
 printf("Interupts not enabled");
 printf(" ");
 mwait(5);
     printf("Welcome to MadOS");
     printf("Copyright (C) 2003  David Karn");
     printf(" ");
     printf("This file is a part of MadOS");
     printf(" ");
     printf("MadOS is free software; you can redistribute it and/or modify");
     printf("it under the terms of the GNU General Public License as published by ");
     printf("the Free Software Foundation; either version 2 of the License, or");
     printf("(at your option) any later version.");
     printf(" ");
     printf("MadOS is distributed in the hope that it will be useful,");
     printf("but WITHOUT ANY WARRANTY; without even the implied warranty of");
     printf("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the ");
     printf("GNU General Public License for more details.");
     printf(" ");
     printf("You should have received a copy of the GNU General Public License");
     printf("along with this program; if not, write to the Free Software");
     printf("Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA");
     printf(" ");
     printf("For more info, look at the COPYING.txt file.");
    mwait(75);
     printf(" ");
      printf(" ");
       printf(" ");
        printf(" ");
         printf(" ");
         printf(" ");
         printf(" ");
textatt=RED_BLACK;
printf("   @@@@@@@@@@    @@@@@@   @@@@@@@    @@@@@@    @@@@@@  ");
printf("   @@@@@@@@@@@  @@@@@@@@  @@@@@@@@  @@@@@@@@  @@@@@@@  ");
printf("   @@! @@! @@!  @@!  @@@  @@!  @@@  @@!  @@@  !@@      ");
printf("   !@! !@! !@!  !@!  @!@  !@!  @!@  !@!  @!@  !@!      ");
printf("   @!! !!@ @!@  @!@!@!@!  @!@  !@!  @!@  !@!  !!@@!!   ");
printf("   !@!   ! !@!  !!!@!!!!  !@!  !!!  !@!  !!!   !!@!!!  ");
printf("   !!:     !!:  !!:  !!!  !!:  !!!  !!:  !!!       !:! ");
printf("   :!:     :!:  :!:  !:!  :!:  !:!  :!:  !:!      !:!  ");
printf("   :::     ::   ::   :::   :::: ::  ::::: ::  :::: ::  ");
printf("    :      :     :   : :  :: :  :    : :  :   :: : :   ");
     printf(" ");
      printf(" ");
       printf(" ");
        printf(" ");
         printf(" ");
         printf(" ");
         printf(" ");
         printf(" ");
mwait(80);
textatt=WHITE_BLACK;
 printf("Sucessfully booted MadOS");
 }
 int mainloop()
 {
    while (1 == 1)
	{
     executecommand(getstring());
	}
 }
#endif
