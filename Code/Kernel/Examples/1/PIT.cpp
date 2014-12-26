#include "PIT.h"

unsigned int PIT::ticks;

void PIT::pitHandler(StackState stack)
{
	ticks++;
}

PIT::PIT(unsigned char chan)
{
	channel = chan;
}

PIT::~PIT()
{
}

void PIT::Enable(unsigned int frequency)
{
	unsigned int divisor = TimerFrequency / frequency;

	outportByte(0x43, channel == 0 ? 0x36 : 0xB6);
	outportByte(0x40 + channel, (unsigned char)(divisor & 0xFF));
	outportByte(0x40 + channel, (unsigned char)((divisor >> 8) & 0xFF));
	AddInterruptHandler(0, pitHandler);
}

void PIT::Disable()
{
	outportByte(0x61, inportByte(0x61) & 0xFC);
}
