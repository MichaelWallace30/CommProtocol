/*
XBee Link that handles all xbee connections.

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
#include <CommProto/network/xbeelink.h>
#include <CommProto/network/xbee.h>
#include <CommProto/debug/comms_debug.h>
#include <CommProto/tools/data_structures/auto_vector.h>

namespace comnet {
namespace network {
namespace experimental {


XBeeLink::XBeeLink()
: xbees(new ::comnet::tools::datastructures::AutoVector<Cluster*>())
, home(new XBee())
{
  this->protocol = ZIGBEE_LINK;
}


bool XBeeLink::InitConnection(const char* port, const char* address, uint32_t baudrate) {
  bool success = home->Initialize(port, baudrate);
  if (!success) {
    COMMS_DEBUG("Home xbee failed to initialize!");
  }
  return success;
}


bool XBeeLink::AddAddress(uint8_t destId, const char* address, uint16_t port) {
  //XBee* xbee = new XBee();
  bool success = false;
  // TODO(Garcia): Need to find devices near this xbee, return with addresses,
  // figure out if address is found, and add xbee to this list.
  return success;
}


bool XBeeLink::RemoveAddress(uint8_t destId) {
  bool success = false;
  // remove xbee from here.
  return success;
}


bool XBeeLink::Send(uint8_t destId, uint8_t* txData, uint32_t txLength) {
  // TODO(Garcia): destId determines the address to send to.
  return home->Send("", txData, txLength);
}


bool XBeeLink::Recv(uint8_t* rxData, uint32_t* rxLength) {
  return home->Recv(rxData, rxLength);
}
} // Experimental
} // Network
} // Comnet 