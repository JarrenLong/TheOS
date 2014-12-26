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
#ifndef COMMAND_H
#define COMMAND_H
int executecommand(char *command)
{
if (strcmp(command, "reboot")==1)
{
printf ("Rebooting MadOS");
mwait(75);
reboot();
scroll();
}
else if (strcmp(command, "help")==1)
{
printf ("commands:");
printf ("___________________________");
printf ("reboot: reboots the computor");
printf ("help: displays a list of commands");
printf ("ASCII: displays ASCII art");
printf ("CHEESE: Your gonna think I'm and idiot dude");
scroll();
}
else if (strcmp(command, "ASCII")==1)
{
printf ("sorry, no ASCII art yet :(");
scroll();
}
else if (strcmp(command, "CHEESE")==1)
{
printf ("CHEESE IS GOOD PEOPLES, EAT LOTSA CHEESE");
scroll();
}
else{
printf("your command of: ");
printf (command);
printf(" is not valid");
scroll();
}
}
#endif
