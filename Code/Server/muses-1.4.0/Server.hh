// Server.hh
// Multi-user, flexible internet server
//
// Copyright (C) 2000 by Paolo Gatti
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

#ifndef SERVER_HH
#define SERVER_HH

#include <list>

namespace muses {

  class ServerPort;
  class TimeoutHandler;
  class ConnectHandler;

  class Server {
  public:
    Server();
    ~Server();

    // Create a new port on which to listen.
    void addPort(unsigned int portnumber, ConnectHandler *handler);

    // Remove a port from the server
    void removePort(ServerPort *port);

    // Read access to the list of active ports
    const std::list<ServerPort*>& ports() const { return _ports; };

    // Wait for something to happen and handle it.
    // Optionally specify a timeout (in microseconds) and provide a timeout
    // handler.
    void run(unsigned int timeout = 0, TimeoutHandler *handler = 0);

    // Shut every port down
    void shutdown();

  private:
    // No copying
    Server(const Server&);
    Server& operator = (const Server&) const;

    // Data members
    std::list<ServerPort*> _ports;
  };

}

#endif /* SERVER_HH */
