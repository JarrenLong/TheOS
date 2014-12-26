#ifndef HTTP_H
#define HTTP_H

#include <vector>
#include "../IPlugin.h"
#include "CRequest.h"
#include "CResponse.h"

//Port 80 HTTP Protocol
class Http : public IPlugin {
public:
    Code Process(std::string data); //Get/set the command flag, handle command
protected:
    //Housekeeping
    bool Clean(std::string data);
    //Command Handlers
    Code Head(std::string opt);
    Code Get(std::string opt);
    Code Put(std::string opt);
    Code Post(std::string opt);
    Code Delete(std::string opt);
    Code Trace(std::string opt);
    Code Options(std::string opt);
    Code Connect(std::string opt);
};

#endif // HTTP_H
