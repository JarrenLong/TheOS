// MusesException.hh
// Exception classes for the muses library
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

#ifndef MUSES_EXCEPTION_HH
#define MUSES_EXCEPTION_HH

#include <stdexcept>

// Muses throws exceptions only in case of critical failures that would affect
// the entire server: for example, when the port the server should listen to
// is already in use. Irrecoverable error conditions regarding single
// connections result instead in the connection being terminated, and in
// the appropriate DisconnectHandler being called.

namespace muses {

  class Server;
  class ServerPort;
  class ConnectHandler;

  //-----------------------------------
  // Root of muses exception hierarchy
  //-----------------------------------
  class MusesException : public runtime_error {
  public:
    MusesException(const std::string& what) : runtime_error(what) { ; };
  };

  //---------------------------------------
  // Critical failure of the entire server
  //---------------------------------------
  class ServerException : public MusesException {
  public:
    ServerException(const std::string& what, muses::Server& server)
      : MusesException(what), _server(server) { ; };
    muses::Server& server() const { return _server; };
  private:
    muses::Server& _server;
  };

  //------------------------------------------
  // Could not open a port on which to listen
  //------------------------------------------
  class AddPortException : public ServerException {
  public:
    AddPortException(const std::string& what,
		     muses::Server& server,
		     unsigned int portNumber,
		     muses::ConnectHandler *handler)
      : ServerException(what, server),
	_portNumber(portNumber),
	_handler(handler) { ; };
    unsigned int portNumber() const { return _portNumber; };
    muses::ConnectHandler *connectHandler() const { return _handler; };
  private:
    unsigned int _portNumber;
    muses::ConnectHandler *_handler;
  };

  //------------------------------
  // Critical error on ServerPort 
  //------------------------------
  class ServerPortException : public MusesException {
  public:
    ServerPortException(const std::string& what, muses::ServerPort& port)
      : MusesException(what), _port(port) { ; };
    muses::ServerPort& port() const { return _port; };
  private:
    muses::ServerPort& _port;
  };
}

#endif /* MUSES_EXCEPTION_HH */
