// RH_LineSplitter.hh
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

#ifndef RH_LINE_SPLITTER_HH
#define RH_LINE_SPLITTER_HH

#include "muses/ReceiveHandler.hh"
#include <string>

namespace muses {

  class LineHandler;

  class RH_LineSplitter : public ReceiveHandler {
  public:
    RH_LineSplitter(LineHandler *lineHandler = 0) :
      _lineHandler(lineHandler), _last(0), _buffer() {};
    virtual ~RH_LineSplitter();

    // Entry point: read some bytes from the connection buffer, return the
    // number of read bytes or zero if more data is needed
    virtual unsigned int operator () (Connection& connection,
				      unsigned int len);

    // Set a parser for each line, return old handler
    LineHandler *setLineHandler(LineHandler *newHandler);

    // Return the current parser
    LineHandler *lineHandler() const { return _lineHandler; };

  private:
    // No copying
    RH_LineSplitter(const RH_LineSplitter&);
    RH_LineSplitter& operator = (const RH_LineSplitter&) const;

    LineHandler *_lineHandler;
    char _last;
    std::string _buffer;
  };

}

#endif /* RH_LINE_SPLITTER_HH */
