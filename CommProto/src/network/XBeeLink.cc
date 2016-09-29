/*
XBee Link that handles all xbee connections.

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
#include <CommProto/network/XBeeLink.h>
#include <CommProto/network/XBee.h>
#include <CommProto/debug/CommsDebug.h>
#include <CommProto/tools/data_structures/AutoVector.h>

namespace Comnet {
namespace Network {
namespace Experimental {


XBeeLink::XBeeLink()
: xbees(new ::Comnet::Tools::DataStructures::AutoVector<XBee*>())
, home(new XBee())
{
}


bool XBeeLink::initConnection(const char* port, const char* address, uint32_t baudrate) {
  bool success = home->initialize(port, baudrate);
  if (!success) {
    COMMS_DEBUG("Home xbee failed to initialize!");
  }
  return success;
}


bool XBeeLink::addAddress(uint8_t destId, const char* address, int16_t port) {
  //XBee* xbee = new XBee();
  bool success = false;
  
  return success;
}


bool XBeeLink::removeAddress(uint8_t destId) {
  bool success = false;
  // remove xbee from here.
  return success;
}


bool XBeeLink::send(uint8_t destId, uint8_t* txData, int32_t txLength) {
  return home->send(destId, txData, txLength);
}


bool XBeeLink::recv(uint8_t* rxData, uint32_t* rxLength) {
  return home->recv(rxData, rxLength);
}
} // Experimental
} // Network
} // Comnet 