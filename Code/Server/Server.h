#include <string>

#include "Socket.h"
#include "./plugins/CPlugin.h"
#include "XPThreads.h"

class Server {
public:
    Server(std::string plugin, unsigned int port, unsigned int maxConn);
    ~Server();

    int Run();
protected:
    void Unpause();
    void Pause();
private:
    bool isRunning;
    unsigned int pNum, cMax;
    std::string plugName;
    CPlugin *plug;
    CReturn data;
    Socket sock, newSock;
    XPThreads *sockets;
};
