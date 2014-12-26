// SendFilter.hh [Abstract base class]
// Perform just-in-time manipulations of the output buffer
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

#ifndef SEND_FILTER_HH
#define SEND_FILTER_HH

namespace muses {

  class Connection;

  class SendFilter {
  public:
    SendFilter() {};
    virtual ~SendFilter() {};

    // Virtual copy constructor
    virtual SendFilter *clone() const = 0;

    // Determine how much space it will take to convert the buffer.
    virtual unsigned int length(const char *buf, unsigned int len) = 0;

    // Perform the actual conversion from "buf" into "dest", return length()
    virtual unsigned int filter(const char *buf, unsigned int len, char *dest)
      = 0;

  };

  // Predefined "plain copy" filter
  extern SendFilter *const raw;

}

#endif /* SEND_FILTER_HH */
