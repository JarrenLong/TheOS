// SF_Telnet.cc
// Change outgoing IAC's into IAC IAC; define 'iac' and 'sf_telnet' globals
//
// Copyright (C) 2001 by Paolo Gatti
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

#include "muses/SF_Telnet.hh"
#include "muses/Connection.hh"
#include <arpa/telnet.h>

using namespace muses;

SendFilter *const muses::sf_telnet = new SF_Telnet();

// Virtual constructor
SendFilter *SF_Telnet::clone() const
{
  return new SF_Telnet(*this);
}

// Determine how much space it will take to convert the buffer.
unsigned int SF_Telnet::length(const char *buf, unsigned int len)
{
  unsigned int iacs = 0;

  for (unsigned int i = 0; i < len; ++i) {
    if (buf[i] == char(IAC)) {
      ++iacs;
    }
  }

  return len + iacs;
}

// Perform the actual conversion from "buf" into "dest"
unsigned int SF_Telnet::filter(const char *buf, unsigned int len, char *dest)
{
  unsigned int i, j;
  unsigned int iacs = 0;
  char c;

  for (i = j = 0; i < len; ++i, ++j) {
    dest[j] = c = buf[i];
    if (c == char(IAC)) {
      dest[++j] = IAC;
      ++iacs;
    }
  }

  return len + iacs;
}

// Send a raw IAC
Connection& muses::iac(Connection& conn)
{
  return conn.insert((unsigned char)(IAC), raw);
}

