// SF_Telnet.hh
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

#ifndef SF_TELNET_HH
#define SF_TELNET_HH

#include "muses/SendFilter.hh"

namespace muses {

  class SF_Telnet : public SendFilter {
  public:
    SF_Telnet() : SendFilter() {};
    virtual ~SF_Telnet() {};

    // Virtual copy constructor
    virtual SendFilter *clone() const;

    // Determine how much space it will take to convert the buffer.
    virtual unsigned int length(const char *buf, unsigned int len);

    // Perform the actual conversion from "buf" into "dest", return length()
    virtual unsigned int filter(const char *buf, unsigned int len, char *dest);

  };

  // Predefined "telnet" filter
  extern SendFilter *const sf_telnet;

  // Send a raw IAC
  Connection& iac(Connection& conn);
}

#endif /* SF_TELNET_HH */
