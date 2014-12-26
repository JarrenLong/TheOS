#ifndef DESCRIPTORTABLES_H
#define DESCRIPTORTABLES_H

struct GDTEntry
{
	unsigned short LimitLow;	//The lower 16 bits of the limit.
	unsigned short BaseLow;		//The lower 16 bits of the base.
	unsigned char BaseMiddle;	//The middle 8 bits of the base.
	unsigned char Access;		//Access flags.
	unsigned char Granularity;
	unsigned char BaseHigh;		//The last 8 bits of the base.
} __attribute__((packed));

//To save definitions, we'll use this for both the GDT and IDT.
struct DescriptorPointer
{
	unsigned short Limit;		//Total size of the descriptor table.
	unsigned int Address;		//Pointer to the start of the descriptor table.
} __attribute__((packed));

extern "C" void Processor_SetGDT(unsigned int gdt);

//A class which sets up the GDT.
class GDT
{
private:
	GDT();
	~GDT();
	static GDTEntry gdt[5];
	static DescriptorPointer gdtPointer;
	static void setEntry(unsigned char id, unsigned short limitLow, unsigned short baseLow, unsigned short baseMiddle, unsigned char access,
		unsigned char granularity, unsigned char baseHigh);
public:
	static void SetupGDT();
};	
#endif
