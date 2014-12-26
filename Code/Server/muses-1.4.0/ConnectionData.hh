// ConnectionData.hh
// Extended data tied to a Connection
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

#ifndef CONNECTION_DATA_HH
#define CONNECTION_DATA_HH

// If your application does not need to bind extra data to Connections,
// ignore this class. Otherwise, store your data in a class that descends
// from ConnectionData, attach it to Connections via setData(), and retrieve
// it in handlers like this:
//
// MyHandler::operator() (Connection& c, ...)
// {
//    MyData *data = dynamic_cast<MyData*>(c.data());
//    if (!data) {
//      // No data, or wrong type of data, available
//    } else {
//      ...
//    }
// }

namespace muses {

  class ConnectionData {
  public:
    ConnectionData() {};
    virtual ~ConnectionData();
  };

}

#endif /* CONNECTION_DATA_HH */
