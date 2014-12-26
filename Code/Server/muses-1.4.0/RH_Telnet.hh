// RH_Telnet.hh
// Specialized ReceiveHandler: implement the telnet protocol, pass on data
// to another ReceiveHandler.
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

#ifndef RH_TELNET_HH
#define RH_TELNET_HH

#include "muses/ReceiveHandler.hh"
#include <string>

namespace muses {

  class RH_Telnet : public ReceiveHandler {
  public:
    RH_Telnet(ReceiveHandler *subHandler = 0);
    virtual ~RH_Telnet();

    // Entry point: read some bytes from the connection buffer, return the
    // number of read bytes or zero if more data is needed
    virtual unsigned int operator () (Connection& connection,
				      unsigned int len);

    // Change sub-handler, return old handler
    ReceiveHandler *setReceiveHandler(ReceiveHandler *newHandler)
    {
      ReceiveHandler *old = _subHandler;
      _subHandler = newHandler;
      return old;
    };

    // Return the current sub-handler
    ReceiveHandler *receiveHandler() const { return _subHandler; };

  protected:
    // Handle telnet commands, return number of interpreted characters or 0 if
    // waiting for more. IAC IAC is already handled by operator().
    // offset -> position of a found IAC
    // This method is meant to be overridden in subclasses.
    virtual unsigned int handleIAC(Connection& connection,
				   unsigned char cmd,
				   unsigned int offset);

  private:
    // No copying
    RH_Telnet(const RH_Telnet&);
    RH_Telnet& operator = (const RH_Telnet&) const;

    ReceiveHandler *_subHandler;
  };

}

#endif /* RH_TELNET_HH */
