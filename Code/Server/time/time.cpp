#include "time.h"

void __declspec(dllexport) GetPlugin( IPlugin **pOut) {
    *pOut = new Time;
}

Code Time::Process(std::string data) {
    time_t rawtime;
    time(&rawtime);
    if (data == "time") {
        char buffer[4];
        sprintf(buffer,"%ld", rawtime);
        return Code(0,std::string(buffer));
    } else if (data == "daytime") {
        struct tm *timeinfo = localtime(&rawtime);
        std::string time = asctime(timeinfo);
        //Strip newline at end
        int f = time.find('\n',0);
        if (f!=-1) {
            time.erase(f,1);
        }
        return Code(0,time);
    }
    return Code(0,"BADCALL");
}
