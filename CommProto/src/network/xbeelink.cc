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
#include <CommProto/network/commxbee.h>
//#include <CommProto/network/znodetable.h>
#include <CommProto/debug/log.h>
#include <CommProto/tools/data_structures/auto_vector.h>


#include <cstring>

namespace comnet {
namespace network {

XBeeLink::XBeeLink()
: home(new CommXBee())
{
  this->protocol = ZIGBEE_LINK;
}


XBeeLink::~XBeeLink() 
{
  free_pointer(home);
}


bool XBeeLink::InitConnection(const char* port, const char* address, uint32_t baudrate) {
  bool success = home->Initialize(port, baudrate);
  if (!success) {
		debug::Log::Message(debug::LOG_NOTIFY, "Home xbee failed to initialize!\n");
  }
  return true;
}


bool XBeeLink::AddAddress(uint8_t destId, const char* address, uint16_t port) {
  bool success = home->AddAddress(destId, address);
  return success;
}


bool XBeeLink::RemoveAddress(uint8_t destId) {
  bool success = home->RemoveAddress(destId);  
  return success;
}


bool XBeeLink::Send(uint8_t destId, uint8_t* txData, uint32_t txLength) {
 bool success = home->Send(destId, txData, txLength);
 return success;
}


bool XBeeLink::Recv(uint8_t* rxData, uint32_t* rxLength) {
 bool success = home->Recv(rxData, *rxLength);
 
 return success;
}
} // Network
} // Comnet 
