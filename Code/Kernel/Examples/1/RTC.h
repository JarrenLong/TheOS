#ifndef RTC_H
#define RTC_H

#include "Common.h"

//This converts a value from BCD. I've chosen to write it as a macro
#define DecodeBCD(value) ((((encoded) >> 4) * 10) + ((encoded) & 0xF))

class RTC
{
private:
	//You need to implement a DateTime object
	static DateTime *date;
	static unsigned int frequency;
	static void rtcHandler(StackState stack);
	static unsigned char readByte(unsigned char offset);
	static void writeByte(unsigned char offset, unsigned char value);
public:
	//This is initially set up for a 1024 Hz interrupt
	RTC(unsigned char rate = 6);
	~RTC();
	DateTime *GetDate();
};
#endif
