#include "Server.h"

Server::Server(std::string plugin, unsigned int port, unsigned int maxConn)
: pNum(port), cMax(maxConn), plugName(plugin) {
    isRunning = false;

    std::string buffer = ".\\Plugins\\";
    buffer += plugName;
    buffer += ".dll";

    plug = new CPlugin;
    plug->Load(buffer);
    plug->Init();

    sock.Create();
    sock.Bind(pNum);
    sock.Listen(cMax);
}

Server::~Server() {
    isRunning = false;

    sock.Close();
    plug->Kill();

    delete plug;
}

int Server::Run() {
    isRunning = true;
    while (isRunning) {
        if (sock.Accept(&newSock)) {
            char buf[65535];
            newSock.RecvLine(buf,65535);

            data = plug->Process(buf);
            std::string msg = data.GetStr();

            newSock.SendText(msg.c_str());
            newSock.Close();
        }
    }
    return 0;
}

void Server::Unpause() {
    isRunning = true;
}

void Server::Pause() {
    isRunning = false;
}
