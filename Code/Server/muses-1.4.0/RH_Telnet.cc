// RH_Telnet.cc
// Specialized ReceiveHandler: implement the telnet protocol, pass on data
// to another ReceiveHandler.
//
// Copyright (C) 2000 by Paolo Gatti
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

#include "muses/RH_Telnet.hh"

#include "muses/Connection.hh"
#include "muses/SF_Telnet.hh"
#include <arpa/telnet.h>

using namespace muses;

RH_Telnet::RH_Telnet(ReceiveHandler *subHandler)
  : _subHandler(subHandler)
{
  ;
}

RH_Telnet::~RH_Telnet()
{
  ;
}

// Entry point: read some bytes from the connection buffer, return the
// number of read bytes or zero if more data is needed
unsigned int RH_Telnet::operator () (Connection& connection, unsigned int len)
{
  const char *buf = connection.buffer();
  unsigned char cmd;
  unsigned int offset, done;

  for (offset = 0; offset < len; ++offset) {
    if ((cmd = buf[offset]) == IAC) {
      
      // Handle IAC <end of buffer>
      if (offset == len - 1) {
	if (_subHandler) {
	  done = (*_subHandler)(connection, offset);
	  return done;
	}
	return offset;
      }

      // Get telnet command
      cmd = buf[offset + 1];

      // Handle IAC IAC
      if (cmd == IAC) {
	connection.removeFromBuffer(offset, 1);
	--len;
	continue;
      }

      // Call sub-handler (up to IAC)
      if (_subHandler && offset) {
	done = (*_subHandler)(connection, offset);
	if (!connection) // subHandler closed the connection
	  return done;
	connection.removeFromBuffer(0, done);
	len -= done;
	offset -= done;
      }

      // Delegate IAC cmd
      unsigned int removed = handleIAC(connection, cmd, offset);
      if (removed == 0)
	return offset;
      connection.removeFromBuffer(offset, removed);
      len -= removed;
      --offset;
    }
  }

  // Anything left?
  if (_subHandler && offset) {
    done = (*_subHandler)(connection, offset);
    return done;
  }

  // No subhandler - drop anything that remains
  return len;
}

// Handle telnet codes, return number of interpreted characters or 0
// offset -> a found IAC
// Minimal implementation for now - refuse all options, eat commands.
// We don't send any option, hence DO x -> WONT x, WILL x -> DONT x
// Ignore anything else.
unsigned int RH_Telnet::handleIAC(Connection& connection,
				  unsigned char cmd,
				  unsigned int offset)
{
  unsigned int len = connection.pendingSize();
  const char *buf = connection.buffer();

  int cmdlen;

  switch (cmd) {
  case DO:
  case DONT:
  case WILL:
  case WONT:
    cmdlen = 2;
    break;

  default:
    // Ignore command; as we deny all options we shouldn't receive SB
    cmdlen = 1;
    break;
  }

  if (len < offset + cmdlen)
    return 0; // Wait for more characters

  switch (cmd) {
  case DO:
    connection
      << iac
      << (unsigned char)(WONT)
      << (unsigned char)(buf[offset+2]);
    break;
  case WILL:
    connection
      << iac
      << (unsigned char)(DONT)
      << (unsigned char)(buf[offset+2]);
    break;
  default:
    break;
  }

  return cmdlen + 1;
}
