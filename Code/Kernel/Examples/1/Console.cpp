#include "Console.h"

uint Console::x = 0;
uint Console::y = 0;

Console::Console()
{
	x = y = 0;
}

Console::~Console()
{
}

void Console::Clear()
{
	ushort *videoMemory = (ushort *)0xB8000;		//Just a pointer to video memory
	uchar attribute = (Black << 4) | (White & 0xF);	//Black background, white text

	for(uint i = 0; i < 80 * 25; i++)				//Remember, 25 rows and 80 columns
	{
		videoMemory[i] = (ushort)(0x20 | (attribute << 8));
	}
}

void Console::WriteChar(char c, ConsoleColour textColour, ConsoleColour backColour)
{
	//See the article for an explanation of this. Don't forget to add support for new lines
	ushort *videoMemory = (ushort *)0xB8000 + 80 * y + x;
	uchar attribute = (backColour << 4) | (textColour & 0xF);

	*videoMemory = (c | (ushort)(attribute << 8));
	x++;
}
