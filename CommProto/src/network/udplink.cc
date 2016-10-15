/*
  UDP Link.

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
#include <CommProto/network/udplink.h>


namespace comnet {
namespace network {



bool UDPLink::InitConnection(const char* port, const char* address, uint32_t baudrate) {
  return local.InitConnection(port, address);
}


bool UDPLink::AddAddress(uint8_t dest_id, const char* address, uint16_t port) {
  return local.AddAddress(dest_id, address, port);
}


bool UDPLink::RemoveAddress(uint8_t dest_id) {
  return local.RemoveAddress(dest_id);
}


bool UDPLink::Send(uint8_t dest_id, uint8_t* txData, uint32_t txLength) {
  return local.Send(dest_id, txData, txLength);
}


bool UDPLink::Recv(uint8_t* rxData, uint32_t* rxLength) {
  return local.Recv(rxData, rxLength);
}
} // namespace Network
} // namespace Comnet