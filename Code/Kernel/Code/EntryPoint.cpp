#include "Kernel.h"

extern "C" int main() {
	Kernel *k;
	k->Init();
	delete k;
	return 0;
}
