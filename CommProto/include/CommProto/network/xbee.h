/*
  XBee class for handling xbee connections.

  Copyright (C) 2016  Mario Garcia.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (At your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __XBEE_H
#define __XBEE_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/connection/serial-config.h>

#include <list>
#include <memory>

namespace comnet {
namespace network {

/**
   XBee communications module. Used for any Xbee protocol connection made to 
   other devices. Still needs work.
*/
class COMM_EXPORT XBee {
public:
  /**
    Constructor that autoinitializes when created. function initialize is automatically called from 
    this constructor. 
   */
  XBee(const char* port, speed_t baudrate);

  XBee();
  /**
     Default destructor.
   */
  ~XBee();
  /**
    Initialize the port on this device.
   */
  bool Initialize(const char* port, speed_t baudrate);
  /**
    Send data over to the destination node.
   */
  bool Send(const char* wpan_addr, uint8_t* txData, uint32_t txLength);
  /**
    Check Receive data.
   */
  bool Recv(uint8_t* rxData, uint32_t* rxLength);
  
 

private:
 
};
} // namespace Network
} // namespace Comnet
#endif // __XBEE_H
