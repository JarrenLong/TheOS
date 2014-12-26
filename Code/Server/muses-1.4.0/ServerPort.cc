// ServerPort.cc
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

#include "muses/ServerPort.hh"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include "muses/Server.hh"
#include "muses/ConnectHandler.hh"
#include "muses/Connection.hh"
#include "muses/MusesException.hh"

using namespace muses;
using std::list;
using std::string;

#ifndef HAVE_SOCKLEN_T
typedef int socklen_t;
#endif

const unsigned int NO_SOCKET = ~0;

ServerPort::ServerPort(Server *server,
		       unsigned int portnumber,
		       ConnectHandler *handler)
  : _server(server),
    _portNumber(portnumber),
    _connHandler(handler),
    _lockCount(0)
{
  // Create a new socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    static const string header
      ("muses::ServerPort::ServerPort() socket() error: ");
    throw AddPortException(header + string(strerror(errno)),
			   *server, portnumber, handler);
  }

  int val = 1;
  if (setsockopt(sock,
		 SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(val)) == -1) {
    close(sock);
    static const string header
      ("muses::ServerPort::ServerPort() setsockopt() error: ");
    throw AddPortException(header + string(strerror(errno)),
			   *server, portnumber, handler);
  }

  // Bind it to the appropriate port
  struct sockaddr_in sai;
  memset((char *)&sai, 0, sizeof(struct sockaddr_in));
  sai.sin_family = AF_INET;
  sai.sin_port = htons(portnumber);
  if (bind(sock, (struct sockaddr *)&sai, sizeof(sai)) == -1) {
    close(sock);
    static const string header
      ("muses::ServerPort::ServerPort() bind() error: ");
    throw AddPortException(header + string(strerror(errno)),
			   *server, portnumber, handler);
  }

  // And be ready to accept connections
  if (listen(sock, 128) == -1) {
    close(sock);
    static const string header
      ("muses::ServerPort::ServerPort() listen() error: ");
    throw AddPortException(header + string(strerror(errno)),
			   *server, portnumber, handler);
  }

  _socketNumber = sock;
}

ServerPort::~ServerPort()
{
  if (socketNumber() != NO_SOCKET)
    throw MusesException("muses::ServerPort destructor called when the "
			 "port was still open");
}

// Detach from server (private)
void ServerPort::clearServer()
{
  _server = 0;
}

ServerPort::Lock::~Lock() // Postponed self-destruction mechanism
{
  if (--_port->_lockCount == 0 &&
      _port->socketNumber() == NO_SOCKET)
    delete _port;
}

// Replace the connection handler, return the old handler
ConnectHandler *ServerPort::setConnectHandler(ConnectHandler *handler)
{
  ConnectHandler *old = _connHandler;
  _connHandler = handler;
  return old;
}

// Someone's trying to connect
void ServerPort::dispatch()
{
  struct sockaddr addr;
  socklen_t size = sizeof(addr);

  // Accept the connection, get the corresponding socket
  int sock = accept(socketNumber(), &addr, &size);

  if (sock == -1) {
    static const string header
      ("muses::ServerPort::dispatch() accept() error: ");
    throw ServerPortException(header + string(strerror(errno)), *this);
  }

  // Build a Connection object
  Connection *connection = new Connection(this, sock);
  _connections.push_back(connection);
  
  // Invoke the ConnectHandler, if any
  if (_connHandler)
    (*_connHandler)(*connection);
}

// Close a connection
void ServerPort::disconnect(Connection *conn)
{
  _connections.remove(conn);
  conn->detach();
  conn->disconnect();
}

// Shut the port down and remove it from the server
void ServerPort::shutdown()
{
  if (_server) {
    _server->removePort(this);
    return;
  }

  if (_socketNumber == NO_SOCKET)
    return;

  // Shut down all connections
  for (list<Connection *>::iterator j = _connections.begin();
	 j != _connections.end();
	 ++j) {
    (*j)->detach();
    (*j)->disconnect();
  }

  _connections.clear();

  // Shut down myself
  if (close(_socketNumber) == -1) {
    static const string header
      ("muses::ServerPort::shutdown() close() error: ");
    throw ServerPortException(header + string(strerror(errno)), *this);
  }

  _socketNumber = NO_SOCKET;

  if (!_lockCount)
    delete this;
}

// Data members
//  unsigned int _portNumber;
//  unsigned int _socketNumber;
//  Server *_server;
//  ConnectHandler *_connHandler;
//  std::list<Connection*> _connections;
