#ifndef SOUND_H
#define SOUND_H

putsound(unsigned int freq)
{
outb(0x43,182);
outw(freq,0x42);
asm("in   0x61,%al");
asm("or   00000011b,%al");
asm("out  %al,0x61");
}


#endif
