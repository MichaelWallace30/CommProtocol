/*
  Serial Link.

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
#include <CommProto/network/seriallink.h>
#include <CommProto/network/serial.h>

#include <CommProto/architecture/macros.h>
#include <CommProto/tools/data_structures/auto_vector.h>

namespace comnet {
namespace network {


using namespace comnet::tools::datastructures;


class SerialConn {
public:
  Serial serial;
  uint8_t id;
};


SerialLink::SerialLink()
: local(NULL)
, connections(allocate_pointer(AutoVector<SerialConn*>))
{
  this->protocol = SERIAL_LINK;
}


SerialLink::~SerialLink()
{
  cleanupListPointerAttributes(connections);
  
  free_pointer(connections);
}


bool SerialLink::InitConnection(const char* port, const char* address, uint32_t baudrate) {
  bool success = false;

  if (!local) {
    local = allocate_pointer(Serial);
    success = local->OpenConnection(port, address, baudrate);
  }

  return success;
}


bool SerialLink::AddAddress(uint8_t dest_id, const char* address, uint16_t port) {
  bool success = false;
  //Serial* conn = allocate_pointer(Serial);
  /*
    TODO(Garcia) : We will need to figure this out later.
  */
  //conn->openConnection(port, address, local->getSerialPort().baudrate);
  return success;
}


bool SerialLink::RemoveAddress(uint8_t dest_id) {
  bool success = false;

  return success;
}


bool SerialLink::Send(uint8_t destId, uint8_t* tx_data, uint32_t tx_length) {
  bool success = false;
#if 0
  for (int32_t i = 0; i < connections->getSize(); ++i) {
    SerialConn* s = connections->at(i);
    if (s->id == destId) {
      success = s->serial.send(destId, txData, txLength);
      break;  
    }
  }
#endif 
  // For now.
  success = local->Send(destId, tx_data, tx_length);
  return success;
}


bool SerialLink::Recv(uint8_t* rx_data, uint32_t* rx_length) {
  bool success = false;
#if 0
  for (int32_t i = 0; i < connections->getSize(); ++i) {
    SerialConn* s = connections->at(i);
    if (s->serial.recv(rxData, rxLength)) {
      success = true;
      break;
    }
  }
#endif
  // for now.
  success = local->Recv(rx_data, rx_length);
  return success;
}
} // namespace Network
} // namespace Comnet