// ReceiveHandler.hh [Abstract base class]
// What should a connection do when receiving input?
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

#ifndef RECEIVE_HANDLER_HH
#define RECEIVE_HANDLER_HH

namespace muses {

class Connection;

class ReceiveHandler {
public:
  ReceiveHandler() {};
  virtual ~ReceiveHandler() {};

  // Entry point: read some bytes from the connection buffer, return the
  // number of read bytes or zero if more data is needed
  virtual unsigned int operator () (muses::Connection& connection,
				    unsigned int len) = 0;

private:
  // No copying
  ReceiveHandler(const ReceiveHandler&);
  ReceiveHandler& operator = (const ReceiveHandler&) const;
};

}

#endif /* RECEIVE_HANDLER_HH */
