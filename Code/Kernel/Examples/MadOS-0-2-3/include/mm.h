/* remap the PIC controller interrupts to our vectors
   rather than the 8 + 70 as mapped by default */
#ifndef MM_H
#define MM_H

#define BLOCK 4096
extern void *sbrk( unsigned);

void *malloc( unsigned nbytes)
{
 static int count = 0;
 static char *ptr = 0;
 union align { double d; unsigned u; void (* f)( void); } align;
 nbytes = (nbytes + (sizeof align - 1))&~( sizeof align - 1);

 if (nbytes <= count) {
  void *p = ptr;
  ptr += nbytes;
  count -= nbytes;
  return p;
 }

 else if (nbytes > BLOCK) {
  void *p = sbrk( nbytes);
  return p == (void *)- 1 ? 0 : p;
 }

 else { /* count < nbytes <= BLOCK */

void *p = sbrk( BLOCK);

 if (p == (void *)- 1)
  return 0;
  count = BLOCK;
  ptr = p;
  return malloc( nbytes); /* no error check? */
 }
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


#endif
