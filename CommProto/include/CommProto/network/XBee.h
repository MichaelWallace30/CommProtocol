/*
  XBee class for handling xbee connections.

  Copyright (C) 2016  Mario Garcia.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __XBEE_H
#define __XBEE_H

#include <CommProto/architecture/connection/xbee_serial.h>

namespace Comnet {
namespace Network {
namespace Experimental {


/**
   XBee communications module. Used for any Xbee protocol connection made to 
   other devices.
*/
class XBee {
public:
  /**
     Default constructor.
   */
  XBee() 
    { }
  /**
     Default destructor.
   */
  ~XBee()
    { }
private:
  /**
   */
  xbee_serial_t serial;
};
} // namespace Experimental
} // namespace Network
} // namespace Comnet
#endif // __XBEE_H
