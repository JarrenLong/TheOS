// RH_LineSplitter.cc
// Specialized ReceiveHandler: split received data into lines and parse each
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

#include "muses/RH_LineSplitter.hh"
#include "muses/LineHandler.hh"
#include "muses/Connection.hh"
#include <string>

using namespace muses;

RH_LineSplitter::~RH_LineSplitter()
{
  ;
}

// Entry point: read some bytes from the connection buffer, return the
// number of read bytes or zero if more data is needed
unsigned int RH_LineSplitter::operator () (Connection& connection,
					   unsigned int len)
{
  const char *buf = connection.buffer(); // Start of current line
  const char *eob = buf + len;

  // During the previous read, a single \n or \r was the last character.
  // See if we need to skip the first character in the new data.
  if (_last && len &&
      (*buf == '\n' || *buf == '\r') &&
      *buf != _last) {
    ++buf;
  };

  const char *pos = buf;

  // We might have read multiple lines, parse them one at a time
  do {

    // Scan the buffer
    while (pos != eob && *pos != '\n' && *pos != '\r')
      ++pos;

    // Partial line - move it to the LineSplitter's buffer and clear the
    // Connection buffer.
    if (pos == eob) {
      _buffer += std::string(buf, pos);
      return len; // Connection buffer is now empty
    }

    // Remember where we are
    const char *eot = pos;

    // Filter out \r\n and \n\r.
    _last = *pos; // Either \r or \n
    ++pos;
    if (pos != eob) {    // There is at least one more character waiting
      if ((*pos == '\n' || *pos == '\r') &&
	  *pos != _last)
	++pos;
      _last = 0;
    }
    // else _last remains non-zero and a \n or \r might be removed next time.

    // Dispatch
    if (_lineHandler) {
      std::string line = _buffer + std::string(buf, eot);
      (*_lineHandler)(connection, line);
    }
    // If there was a partial line from the previous iteration, clear it.
    _buffer.erase();

    // Throw away whatever has been read
    buf = pos;

    // End of do-loop: examined the entire buffer, or connection was closed
  } while (pos != eob && connection);

  // And free the Connection buffer
  return len;
}

// Set a parser for each line, return old handler
LineHandler *RH_LineSplitter::setLineHandler(LineHandler *newHandler)
{
  LineHandler *old = _lineHandler;
  _lineHandler = newHandler;
  return old;
}

