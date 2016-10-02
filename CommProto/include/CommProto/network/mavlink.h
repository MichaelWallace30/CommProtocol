/* 
  MavLink Interface for wrapping MavLink Protocol.

  Copyright (C) 2016  Michael Wallace, Mario Garcia.

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
#ifndef __MAVLINK_H
#define __MAVLINK_H


#include <CommProto/architecture/os/include_defines.h>

#include <CommProto/network/CommsLink.h>

namespace Comnet {
namespace Network {
namespace Experimental {


/**
   MavLink is an object that holds all MavLink Connections. Right now it is still in the works.
   THIS CLASS IS NOT READY YET. IMPLEMENTATION STILL NEEDS TO BE DONE.
*/
class MavLink : public CommsLink {
public:
  /**
     Calls Constructor for this object.
   */
  MavLink();
  /**
     Calls Destructor for this object.
   */
  ~MavLink();
  /**
     Initializes the connection for this node.
   */
  bool initConnection(const char* port = NULL, const char* = NULL, uint32_t baudrate = 0);
  /**
     Adds an address from which to communicate to.
   */
  bool addAddress(uint8_t destId, const char* address = NULL, uint16_t port = 0);
  /**
     Removes a communication address.
   */
  bool removeAddress(uint8_t destId);
  /**
     Sends a packet to the communication address.
   */
  bool send(uint8_t destId, uint8_t* txData, int32_t txLength);
  /**
     Receives a packet from a communication address.
   */
  bool recv(uint8_t* rxData, uint32_t* rxLength);
private:
  
};
} // namespace Experimental.
} // namespace Network
} // namespace Comnet
#endif // __MAVLINK_H
