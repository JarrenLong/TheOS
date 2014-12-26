// Connection.hh
// A connection to the server
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

#ifndef CONNECTION_HH
#define CONNECTION_HH

#include <fstream>
#include <vector>

namespace muses {

  class ServerPort;
  class ReceiveHandler;
  class DisconnectHandler;
  class ConnectionData;
  class SendFilter;

  class Connection : public std::ostream {
    friend class ServerPort;
    friend class Server;

  private:
    // Connections are only created and destroyed by their ServerPort
    Connection(ServerPort *port, unsigned int sock);
    ~Connection();
    void detach();   // Detach from ServerPort
    bool dispatch(); // Some data arrived - return true if there might be more.

    class Lock { // Postponed destruction mechanism
    public:
      Lock(Connection *c) : _connection(c) { ++c->_lockCount; };
      ~Lock();
    private:
      Connection *_connection;
    };
    friend class Connection::Lock;

    class sockbuf : public std::streambuf {
      friend class muses::Connection;
    public:
      sockbuf(unsigned int sock) : std::streambuf(), _socketNumber(sock)
      {
	// Tell streambuf that the buffer is 1 char shorter than it really is.
	// This will be used for an optimization in overflow().
	setp(_buf, _buf + BUFSIZE - 1);
	_filters.push_back(0);      // By default, don't translate output
      };
      virtual ~sockbuf() {};
      virtual int sync();
      virtual int overflow(int c = EOF);
      unsigned int pos() const { return pptr() - pbase(); };
      void addFilter(SendFilter *filter);
    private:
      unsigned int _socketNumber;
      static const int BUFSIZE = 1024;
      char _buf[BUFSIZE];
      vector<SendFilter *> _filters;
      vector<unsigned int> _filterEnd;
    };

  public:
    // Return the port that accepted this connection
    ServerPort *port()             { return _port; };
    const ServerPort *port() const { return _port; };

    // Read access to the socket number
    unsigned int socketNumber() const { return _sockbuf._socketNumber; };

    // Read access to the receive handler
    ReceiveHandler *receiveHandler() const { return _recHandler; };

    // Replace the receive handler, return the old handler
    ReceiveHandler *setReceiveHandler(ReceiveHandler *recHandler);

    // Read access to the disconnection handler
    DisconnectHandler *disconnectHandler() const { return _discHandler; };

    // Replace the disconnection handler, return the old handler
    DisconnectHandler *setDisconnectHandler(DisconnectHandler *handler);

    // Read access to the receive buffer
    unsigned int pendingSize() const { return _pendingSize; };
    const char *buffer() const { return _buffer; };

    // Remove some data from the receive buffer
    void removeFromBuffer(unsigned int offset, unsigned int size);

    // Size of the receive buffer
    unsigned int bufferSize() const { return _bufferSize; };
    void setBufferSize(unsigned int maxSize);

    // Replace the output filter for future output.
    // The connection makes a copy of the argument; the argument may be
    // safely changed or deleted.
    void setSendFilter(SendFilter *filter)
    { _sockbuf.addFilter(filter); };

    // Insert a custom-filtered item into the Connection buffer;
    // then, restore the original filter.
    template <class T> Connection& insert(T what, SendFilter *filter);

    // Force a disconnection
    void disconnect();

    // Is the Connection still valid?
    operator bool() const { return _port != 0; };
    bool operator !() const { return _port == 0; };

    // Read/write pointer to extended data
    ConnectionData *data() { return _data; };
    const ConnectionData *data() const { return _data; };
    void setData(ConnectionData *data) { _data = data; };

  private:
    // No copying
    Connection(const Connection&);
    Connection& operator = (const Connection&) const;

    // Data members
    ServerPort *_port;
    ReceiveHandler *_recHandler;
    DisconnectHandler *_discHandler;
    ConnectionData *_data;

    unsigned int _bufferSize;
    unsigned int _pendingSize;
    char *_buffer;

    unsigned int _lockCount;

    sockbuf _sockbuf;
  };

  // Insert a custom-filtered object into the stream
  template <class T> Connection& Connection::insert(T what, SendFilter *filter)
  {
    SendFilter *old = _sockbuf._filters.back();
    unsigned int p = _sockbuf.pos();

    // If the current filter is superfluous, some special handling is needed
    // to protect the "old" filter from an early deletion. It should either
    // be cloned and then deleted, or the insertion should be done by hand
    // like here:
    if ((_sockbuf._filterEnd.empty() && p == 0) ||
	(!_sockbuf._filterEnd.empty() && p == _sockbuf._filterEnd.back())) {

      if (filter)
	filter = filter->clone();  // Don't clone a null filter

      // Special case: Caller requested to add a null filter after a
      // superfluous filter, and there is already a null filter before
      // the superfluous one. Drop the superfluous filter.
      if (!filter &&
	  (p = _sockbuf._filters.size()) > 1 && !_sockbuf._filters[p - 2]) {
	_sockbuf._filters.pop_back();
	_sockbuf._filterEnd.pop_back();
      }
      // Normal case: replace the superfluous filter with the new one
      else
	_sockbuf._filters.back() = filter; // Replace the old filter

      (*this) << what;                     // Send the object.
      _sockbuf._filters.push_back(old);    // Restore the old filter
      _sockbuf._filterEnd.push_back(_sockbuf.pos());
    }
    // Current filter is not superfluous: the simple way will work.
    else {
      (*this) << filter << what << old;
    }

    return *this;
  }
}

// Overload operator << to accept Connection manipulators.

// This is done by overloading << on ostream&, rather than Connection&:
// otherwise, "connection << whatever << iac" won't compile correctly,
// since "connection << whatever" is no longer a Connection&.
// Suggestions on how to improve this would be welcome.

// Enable Connection-specific manipulators (similar to endl, flush...)
// Throw an exception in case a Connection manipulator is used on a
// non-Connection.
inline std::ostream&
operator << (std::ostream& stream,
	     muses::Connection& (*func)(muses::Connection&))
{
  muses::Connection& conn =
    dynamic_cast<muses::Connection&>(stream); // Throw bad_cast on failure
  return func(conn);
};

// Inline change of output filter. Print address if stream is not a Connection.
inline std::ostream&
operator << (std::ostream& stream, muses::SendFilter *filter)
{
  muses::Connection* conn = dynamic_cast<muses::Connection*>(&stream);
  if (conn)
    conn->setSendFilter(filter);
  else
    stream << static_cast<const void *>(filter);

  return stream;
}

#endif /* CONNECTION_HH */
