#ifndef TIME_H
#define TIME_H

#include <time.h>
#include "../IPlugin.h"

class Time : public IPlugin {
public:
    Code Process(std::string data);
};

#endif // TIME_H
