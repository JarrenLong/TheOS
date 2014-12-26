// Connection.cc
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

#include "muses/Connection.hh"
#include "muses/MusesException.hh"

#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <sys/socket.h>

using std::string;

#include "muses/ServerPort.hh"
#include "muses/ReceiveHandler.hh"
#include "muses/DisconnectHandler.hh"
#include "muses/SendFilter.hh"

using namespace muses;

const int DEFAULT_BUFFER_SIZE = 1024;
const unsigned int NO_SOCKET = ~0;

Connection::Connection(ServerPort *port, unsigned int sock)
  : std::ostream(&_sockbuf),
    _port(port),
    _data(0),
    _bufferSize(DEFAULT_BUFFER_SIZE),
    _pendingSize(0),
    _recHandler(0),
    _discHandler(0),
    _lockCount(0),
    _sockbuf(sock)
{
  _buffer = new char[_bufferSize];
}

Connection::~Connection()
{
  if (socketNumber() != NO_SOCKET)
    throw MusesException("muses::Connection destructor called when the "
			 "connection was still open");
  delete [] _buffer;
}

void Connection::detach()
{
  _port = 0;
}

Connection::Lock::~Lock() // Postponed self-destruction mechanism
{
  if (--_connection->_lockCount == 0 &&
      _connection->socketNumber() == NO_SOCKET)
    delete _connection;
}

// Output buffering and filtering
int Connection::sockbuf::sync()
{
  if (pbase() == pptr() || _socketNumber == NO_SOCKET)
    return 0;

  char *toSend;
  unsigned int len;
  bool translated;

  if (_filterEnd.empty() && _filters[0] == 0) {
    // No filter, no manipulation
    toSend = pbase();
    len = pptr() - toSend;
    translated = false;
  }
  else {
    // Manipulate the output.
    // Phase 1: Determine length of translated buffer.
    unsigned int begin = 0;
    unsigned int origSize;
    int i;
    char *c = pbase();
    SendFilter *sf;
    translated = true;

    // Calculate translated length of each buffer partition.
    len = 0;
    for (i = 0; i < _filterEnd.size(); ++i) {
      sf = _filters[i];
      origSize = _filterEnd[i] - begin;
      if (sf)
	len += sf->length(c + begin, origSize);
      else // No translation
	len += origSize;
      begin = _filterEnd[i];
    }
    // Last partition is special
    sf = _filters[i];
    origSize = pptr() - c - begin;
    if (sf)
      len += sf->length(c + begin, origSize);
    else // No translation
      len += origSize;

    // Phase 2: allocate a new buffer
    toSend = new char[len];

    // Phase 3: translate each partition
    begin = 0;
    unsigned int tbegin = 0; // Translation
    for (i = 0; i < _filterEnd.size(); ++i) {
      sf = _filters[i];
      origSize = _filterEnd[i] - begin;
      if (sf)
	tbegin += sf->filter(c + begin, origSize, toSend + tbegin);
      else { // No translation
	memcpy(toSend + tbegin, c + begin, origSize);
	tbegin += origSize;
      }
      begin = _filterEnd[i];
    }
    // Last partition is special
    sf = _filters[i];
    origSize = pptr() - c - begin;
    if (sf)
      tbegin += sf->filter(c + begin, origSize, toSend + tbegin);
    else { // No translation
      memcpy(toSend + tbegin, c + begin, origSize);
      tbegin += origSize;
    }

    // ASSERT: tbegin == len

    // Phase 4: release old filters
    if (!_filterEnd.empty()) {
      for (i = 0; i < _filterEnd.size(); ++i) // Keep the most recent
	delete _filters[i];
      _filters.clear();
      _filters.push_back(sf);
      _filterEnd.clear();
    }
  }

  // Send stuff to the socket. Use global write(), not ostream::write().
  unsigned int written = ::write(_socketNumber, toSend, len);
  // Release translation memory
  if (translated)
    delete [] toSend;
  // Clear the original buffer
  setp(pbase(), epptr()); // Clear the buffer
  // Return EOF on error
  if (written != len)
    return EOF;
  return 0;
}

int Connection::sockbuf::overflow(int c)
{
  // sockbuf thinks that the buffer is one char shorter than it really is
  // (see the sockbuf constructor). As a consequence, overflow() is called
  // when there is still one free char in the buffer.
  // Put c in that extra byte, and dump the entire buffer in one step.
  *pptr() = c;
  pbump(1);
  return sync();
}

// Replace the receive handler, return the old handler
ReceiveHandler *Connection::setReceiveHandler(ReceiveHandler *recHandler)
{
  ReceiveHandler *old = _recHandler;
  _recHandler = recHandler;
  return old;
}

// Replace the disconnection handler, return the old handler
DisconnectHandler *Connection::setDisconnectHandler(DisconnectHandler *handler)
{
  DisconnectHandler *old = _discHandler;
  _discHandler = handler;
  return old;
}

// Some data arrived - return true if there might be more.
// Do not try to read everything instantly, give other connections a chance
// to send data as well.
bool Connection::dispatch()
{
  // Make sure the buffer is not full
  if (_pendingSize == _bufferSize)
    {
      // Warn and...
      disconnect();
      return false;
    }

  // Read everything into the buffer
  int len = recvfrom(socketNumber(), _buffer, _bufferSize - _pendingSize,
		     0, 0, 0);

  if (len <= 0)
    {
      disconnect();
      return false;
    }

  bool tryAgain = (len == _bufferSize - _pendingSize);

  _pendingSize += len;

  // Call the receive handler (if any) or throw it all away
  if (_recHandler) {
    unsigned int got = (*_recHandler)(*this, _pendingSize);
    removeFromBuffer(0, got);
  }
  else // No handler, throw it all away
    _pendingSize = 0;

  return tryAgain;
}

// Remove some data from the buffer
void Connection::removeFromBuffer(unsigned int offset, unsigned int size)
{
  if (offset > _pendingSize)
    offset = _pendingSize;
  if (offset + size > _pendingSize)
    size = _pendingSize - offset;

  if (size == _pendingSize)
    _pendingSize = 0;
  else { // Shift rest of the buffer
    unsigned int end = offset + size;
    memmove(_buffer + offset, _buffer + end, _pendingSize - end);
    _pendingSize -= size;
  }
}

// Size limits on the receive buffer
void Connection::setBufferSize(unsigned int maxSize)
{
  // Can't decrease the buffer size below the current contents
  if (maxSize < _pendingSize)
    maxSize = _pendingSize;

  if (maxSize == _bufferSize)
    return;

  char *newBuffer = new char[maxSize];
  memmove(newBuffer, _buffer, _pendingSize);
  delete [] _buffer;
  _buffer = newBuffer;
  _bufferSize = maxSize;
}

// Replace the output filter for future output.
// The connection makes a copy of the argument; the argument may be
// safely changed or deleted.
void Connection::sockbuf::addFilter(SendFilter *filter)
{
  // Caller might delete or change the argument before sockbuf is done with it.
  // To be safe, store a copy. The null filter (being unique) is an exception.
  if (filter)
    filter = filter->clone();
  // Special case: replacing no-filter with no-filter
  else if (_filters.back() == 0)
    return;

  // If the previous filter does not have to translate anything, replace it.
  // Otherwise stack the new filter.
  unsigned int p = pos();
  if (p == 0 || (_filterEnd.size() > 0 && p == _filterEnd.back())) {
    // Either the buffer is empty, or the previous filter is superfluous
    SendFilter *old = _filters.back();
    if (old)
      delete old;
      // Special case: Caller requested to add a null filter after a
      // superfluous filter, and there is already a null filter before
      // the superfluous one. Drop the superfluous filter.
    if (!filter && (p = _filters.size()) > 1 && !_filters[p - 2]) {
      _filters.pop_back();
      _filterEnd.pop_back();
    }
    // Normal case: replace the superfluous filter with the new one
    else
      _filters.back() = filter;
  }
  // Current filter is not superfluous. Stack the new one after it.
  else {
    _filters.push_back(filter);
    _filterEnd.push_back(p);
  }
}

// Force a disconnection
void Connection::disconnect()
{
  if (socketNumber() == NO_SOCKET)
    return;

  if (_port) {
    _port->disconnect(this);
    return;
  }

  // Notify the user
  if (_discHandler)
    (*_discHandler)(*this);

  // Close the socket
  close(socketNumber());
  _sockbuf._socketNumber = NO_SOCKET;

  // And self-destruct
  if (!_lockCount)
    delete this;
}

// Data members
//  ServerPort *_port;
//  ReceiveHandler *_recHandler;
//  DisconnectHandler *_discHandler;
//  unsigned int _bufferSize;
//  unsigned int _pendingSize;
//  char *_buffer;
