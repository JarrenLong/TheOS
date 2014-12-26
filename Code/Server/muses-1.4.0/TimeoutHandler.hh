// TimeoutHandler.hh [Abstract base class]
// What should the server do if no data arrives?
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

#ifndef TIMEOUT_HANDLER_HH
#define TIMEOUT_HANDLER_HH

namespace muses {

class Server;

  class TimeoutHandler {
  public:
    TimeoutHandler() {};
    virtual ~TimeoutHandler() {};

    // Entry point
    virtual void operator () (Server& server) = 0;

  private:
    // No copying
    TimeoutHandler(const TimeoutHandler&);
    TimeoutHandler& operator = (const TimeoutHandler&) const;
  };

}

#endif /* TIMEOUT_HANDLER_HH */
