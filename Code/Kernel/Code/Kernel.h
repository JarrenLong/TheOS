#ifndef KERNEL_H
#define KERNEL_H

#include "x86.h"
#include "icxxabi.h"
#include "types.h"
#include "GDT.h"
#include "ISR.h"
#include "IRQ.h"

class Kernel {
public:
    Kernel() {}
    ~Kernel() {}

    int Init() {
        /* Get into long mode, then...
        gdt = new GDT;
        //Remap PICS
        idt = new IDT;
        isr = new ISR;
        irq = new IRQ;

        screen = new Graphics;
        fs = new Filesystem;
        sound = new Sound;
        network = new Network;

        delete network;
        elete sound;
        delete fs;
        delete screen;

        delete irq;
        delete isr;
        delete idt;
        delete gdt;
        */
        return 0;
    }
private:
    /*
    GDT *gdt;
    IDT *idt;
    ISR *isr;
    IRQ *irq;
    Graphics *screen;
    Filesystem *fs;
    Sound *sound;
    Network *network;
    */
};

#endif

/*
putsound(unsigned int freq)
{
outb(0x43,182);
outw(freq,0x42);
asm("in   0x61,%al");
asm("or   00000011b,%al");
asm("out  %al,0x61");
}

void paging()
{
	unsigned long *page_directory	= (unsigned long *) 0x9C000;
	unsigned long *page_table		= (unsigned long *) 0x9D000; // the page table comes right after the page directory
	unsigned long address=0; // holds the physical address of where a page is
	unsigned int i;

	// map the first 4MB of memory
	for(i=0; i<1024; i++)
	{
		page_table[i] = address | 3; // attribute set to: supervisor level, read/write, present(011 in binary)
		address = address + 4096; // 4096 = 4kb
	};

	// fill the first entry of the page directory
	page_directory[0] = page_table; // attribute set to: supervisor level, read/write, present(011 in binary)
	page_directory[0] = page_directory[0] | 3;
	// fill the rest of the page directory
	for(i=1; i<1024; i++)
	{
		page_directory[i] = 0 | 2; // attribute set to: supervisor level, read/write, not present(010 in binary)
	};

	// write_cr3, read_cr3, write_cr0, and read_cr0 all come from the assembly functions
	write_cr3(page_directory); // put that page directory address into CR3
	write_cr0(read_cr0() | 0x80000000); // set the paging bit in CR0 to 1

	// go celebrate or something 'cause PAGING IS ENABLED!!!!!!!!!!
};
*/
