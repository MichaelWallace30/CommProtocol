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
  bool success = false;
  std::unique_ptr<UDP> udp = local.Connect(dest_id, address, port);
  if (udp != nullptr) {
    clients[dest_id] = std::move(udp);
    success = true;
  }
  return success;
}


bool UDPLink::RemoveAddress(uint8_t dest_id) {
  bool success = false;
  if (clients.find(dest_id) != clients.end()) {
    clients.erase(dest_id);
    success = true;
  }
  return success;
}


bool UDPLink::Send(uint8_t dest_id, uint8_t* txData, uint32_t txLength) {
  bool success = false;
  if (clients.find(dest_id) != clients.end()) {
    success = clients[dest_id]->Send(txData, txLength);
  }
  return success;
}


bool UDPLink::Recv(uint8_t* rxData, uint32_t* rxLength) {
  bool success = false;
  /**
    TODO(Anybody): We need a proper data structure for handling recv from multiple
                  nodes.
  */
  for (std::map<uint8_t, std::unique_ptr<UDP> >::iterator it = clients.begin(); 
            it != clients.end(); ++it) {
    if (it->second && it->second->Recv(rxData, rxLength)) {
      success = true;
      break;
    }
  }
  return success;
}
} // namespace Network
} // namespace Comnet