/* 
    This is the error file for the MadOS kernel, it contains error functions for the MadOS kernel
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

#ifndef ERROR_H
#define ERROR_H
#include "main.h"

int error( char* msg, int errlev );
void mdie();

int error( char* msg, int level )
  {
  clearscreen();
  textatt = RED_BLACK;
  if(level == 1)
    {
    printf("Could not execute the command, ");
    printf("error:");
    printf(msg);
    }
  else if(level == 2)
    {
    printf("Invalid command, ");
    printf(msg);
    }
  else if(level == 3)
    {
    printf("Fatal Error detected, Stopping all procedures, \nfreezing,");
    printf("error:");
    printf(msg);
    mdie();
    }
  else if(level == 4)  //exeption
    {
    printf("Exeption:");
    printf(msg);
    mdie();
    }
  }


void mdie()
{
    asm("cli");
    asm("hlt");
}
#endif

