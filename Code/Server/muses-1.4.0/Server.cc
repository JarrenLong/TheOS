// Server.cc
// Multi-user, flexible internet server
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

#include "muses/Server.hh"

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include "muses/ServerPort.hh"
#include "muses/Connection.hh"
#include "muses/TimeoutHandler.hh"
#include "muses/MusesException.hh"

using namespace muses;
using std::list;
using std::string;

Server::Server()
{
  // Nothing to do.
}

Server::~Server()
{
  shutdown();
}

// Add a new port on which to listen.
void Server::addPort(unsigned int portnumber, ConnectHandler *handler)
{
  ServerPort *port = new ServerPort(this, portnumber, handler);
  _ports.push_back(port);
}

// Remove a port from the server
void Server::removePort(ServerPort *port)
{
  _ports.remove(port);
  port->clearServer();
  port->shutdown();
}

// Wait for something to happen and handle it.
// Optionally specify a timeout (in microseconds) and provide a timeout
// handler.

void Server::run(unsigned int timeout, TimeoutHandler *handler)
{
  // Prepare the list of sockets on which to wait:
  fd_set input_set, output_set, exception_set;
  FD_ZERO(&input_set);
  FD_ZERO(&output_set);
  FD_ZERO(&exception_set);
  int maxSocket = 0;

  // For each port,
  for (list<ServerPort *>::iterator i = _ports.begin();
       i != _ports.end();
       ++i) {

    ServerPort *p = *i;

    // The port's listen socket...
    int sock = p->socketNumber();
    FD_SET(sock, &input_set);
    FD_SET(sock, &exception_set);
    if (maxSocket <= sock)
      maxSocket = sock + 1;

    // ... and any active connection.
    const list<Connection *>& connections = p->connections();
    for (list<Connection *>::const_iterator j = connections.begin();
	 j != connections.end();
	 ++j) {

      Connection *c = *j;
      int sock = c->socketNumber();
      FD_SET(sock, &input_set);
      FD_SET(sock, &exception_set);
      if (maxSocket <= sock)
	maxSocket = sock + 1;
    }
  }

  // Set up timeout
  struct timeval tv;
  tv.tv_sec = timeout / 1000000;
  tv.tv_usec = timeout % 1000000;
  struct timeval zero_time;
  zero_time.tv_sec = zero_time.tv_usec = 0;

  // Wait for something to happen
  int result = select(maxSocket, &input_set, &output_set, &exception_set, &tv);

  // Error?
  if (result == -1) {
    static const string header("muses::Server::run() - select error: ");
    throw ServerException(header + string(strerror(errno)), *this);
  }

  // Timeout?
  else if (result == 0)
    {
      if (handler)
	(*handler)(*this);
      return;
    }

  // Scan ports and connections, call a dispatcher on those that received
  // something.
  for (list<ServerPort *>::iterator i = _ports.begin();
       i != _ports.end() && result;
       ++i) {

    ServerPort *p = *i;
    ServerPort::Lock portLock(p); // Don't delete the port while we use it

    // Check the port's listen socket...
    if (FD_ISSET(p->socketNumber(), &input_set)) {
      --result;
      fd_set listen_set;
      FD_ZERO(&listen_set);
      FD_SET(p->socketNumber(), &listen_set);
      // Keep accepting connections as long as there are clients waiting
      int r;
      while (r = select(p->socketNumber() + 1, &listen_set, 0, 0, &zero_time))
	{
	  static const string header
	    ("muses::Server::run() - select (pre-accept) error: ");
	  if (r == -1)
	    {
	      throw ServerException(header + string(strerror(errno)), *this);
	    }
	  p->dispatch();
	}
    }

    // ... and its connections.
    const list<Connection *>& connections = p->connections();
    for (list<Connection *>::const_iterator j = connections.begin();
	 j != connections.end() && result;
	 ++j) {

      Connection *c = *j;
      if (FD_ISSET(c->socketNumber(), &exception_set))
	{
	  --result;
	  c->disconnect();
	}
      else if (FD_ISSET(c->socketNumber(), &input_set))
	{
	  --result;
	  // A user-provided dispatcher might cause loss of link.
	  // Postpone deletion of the Connection object...
	  Connection::Lock lock(c);
	  c->dispatch();
	}
      // ... until here.
    }
  }
}

// Shut every port down
void Server::shutdown()
{
  for (list<ServerPort *>::iterator i = _ports.begin();
       i != _ports.end();
       ++i) {
    (*i)->clearServer();
    (*i)->shutdown();
  }
  _ports.clear();
}

// Data members
//  std::list<ServerPort*> _ports;
