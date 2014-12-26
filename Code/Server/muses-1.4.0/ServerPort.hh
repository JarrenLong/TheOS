// ServerPort.hh
// A port on which to listen for connections.
//
// Copyright (C) 2000, 2001 by Paolo Gatti
//
// This file is part of the muses library.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef SERVER_PORT_HH
#define SERVER_PORT_HH

#include <list>

namespace muses {

  class Server;
  class ConnectHandler;
  class Connection;

  class ServerPort {
    friend class Server;

  private: // ServerPorts are only created and destroyed by their Server
    ServerPort(Server *server,
	       unsigned int portnumber,
	       ConnectHandler *handler);
    ~ServerPort();
    void clearServer(); // Detach from server

    class Lock { // Postponed destruction mechanism
    public:
      Lock(ServerPort *p) : _port(p) { ++p->_lockCount; };
      ~Lock();
    private:
      ServerPort *_port;
    };
    friend class ServerPort::Lock;

  public:
    // Return the server this port belongs to, or zero
    Server *server()             { return _server; };
    const Server *server() const { return _server; };

    // Read access to the port number
    unsigned int portNumber() const { return _portNumber; };

    // Read access to the socket number
    unsigned int socketNumber() const { return _socketNumber; };

    // Read access to the connection handler
    ConnectHandler *connectHandler() const { return _connHandler; };

    // Replace the connection handler, return the old handler
    ConnectHandler *setConnectHandler(ConnectHandler *handler);

    // Access to the list of active connections
    const std::list<Connection*>& connections() { return _connections; };

    // Close a connection
    void disconnect(Connection *conn);

    // Shut the port down and remove it from the server
    void shutdown();

  private:
    // No copying
    ServerPort(const ServerPort&);
    ServerPort& operator = (const ServerPort&) const;

    // Someone's trying to connect
    void dispatch();

    // Data members
    unsigned int _portNumber;
    unsigned int _socketNumber;
    Server *_server;
    ConnectHandler *_connHandler;
    std::list<Connection*> _connections;
    unsigned int _lockCount;
  };

}

#endif /* SERVER_PORT_HH */
