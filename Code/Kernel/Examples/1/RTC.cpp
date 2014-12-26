#include "RTC.h"

DateTime *RTC::date;
unsigned int RTC::frequency;

void RTC::rtcHandler(StackState stack)
{
	static unsigned int ticks = 0;

	ticks++;
	//1024 Hz = 1024 interrupts per second
	if(ticks % frequency == 0)
	{
		date->AddSeconds(1);
		//To reduce the likelihood of an overflow ocurring, we reset the ticks variable.
		//This means we can run the OS for longer without a crash
		ticks = 0;
	}
	//Whether you do anything with this is up to you. I don't have a use for it.
	readByte(0xC);
}

unsigned char RTC::readByte(unsigned char offset)
{
	outportByte(0x70, offset);
	return inportByte(0x71);
}

void RTC::writeByte(unsigned char offset, unsigned char value)
{
	outportByte(0x70, offset);
	outportByte(0x71, value);
}

RTC::RTC(unsigned char rate)
{
	unsigned char registerB = readByte(0xB) | 0x40;
	unsigned char registerA = readByte(0xA) & 0xF0

	//Read the time and date from the CMOS
	date = new DateTime();
	date->Year = readByte(0x9) + 2000;
	date->Month = readByte(0x8);
	date->Day = readByte(0x7);
	date->Hour = readByte(0x4);
	date->Minute = readByte(0x2);
	date->Second = readByte(0x0);

	//If bit 3 is set, we need to decode the time and date
	if((registerB & 0x4) == 0x4)
	{
		date->Year = DecodeBCD(date->Year);
		date->Month = DecodeBCD(date->Month);
		date->Day = DecodeBCD(date->Day);
		date->Hour = DecodeBCD(date->Hour);
		date->Minute = DecodeBCD(date->Minute);
		date->Second = DecodeBCD(date->Second);
	}
	//Install an interrupt handler
	InstallInterruptHandler(8, rtcHandler);
	//Compute the frequency and set the rate
	frequency = 0x8000 >> (rate - 1);
	writeByte(0xA, registerA | rate);
	//Enable interrupt
	writeByte(0xB, registerB);
}

RTC::~RTC()
{
}

DateTime *RTC::GetDate()
{
	return date;
}
