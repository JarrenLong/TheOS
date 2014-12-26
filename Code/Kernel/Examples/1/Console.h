#ifndef Console_h
#define Console_h
#include "Common.h"

//This gives us a little type-safety, so that we don't write a bad value somewhere along the lines
enum ConsoleColour
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	LightBrown = 14,
	White = 15
};

//This is just a basic console class. You'll want to add support for strings, newlines, formatted strings and integers	
class Console
{
private:
	static uint x;
	static uint y;
public:
	Console();
	~Console();
	void Clear();
	void WriteChar(char c, ConsoleColour textColour, ConsoleColour backColour);
};
#endif
