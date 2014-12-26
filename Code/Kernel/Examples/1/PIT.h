#ifndef PIT_H
#define PIT_H

#include "Common.h"

#define TimerFrequency 1193180

class PIT
{
private:
	static void pitHandler(StackState stack);
	unsigned char channel;
	unsigned int frequency;
	static unsigned int ticks;
public:
	PIT(unsigned char chan);
	~PIT();
	void Enable(unsigned int hz);
	void Disable();
};
#endif
