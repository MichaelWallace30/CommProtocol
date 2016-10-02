/*
  Link for Serial connections.

  Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia.

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
#ifndef __SERIAL_LINK_H
#define __SERIAL_LINK_H

#include <CommProto/network/commslink.h>

#include <CommProto/tools/data_structures/interface/interface_list.h>

namespace comnet {
namespace network {


class Serial;
class SerialConn;
using comnet::tools::datastructures::interface::List;


/**
  Serial link for Communications protocol. Allows multiple connections via Serial.
  This is currently not the case, so we will keep it like this for future research on 
  swarm communications.
*/
class SerialLink : public CommsLink {
public:
  /**
   */
  SerialLink();
  /**
   */
  ~SerialLink();
  /** Opens comport sets if it is connected on scucces, address is not need just use "" argument
  Returns false if opening comport fails*/
  virtual bool initConnection(const char* port = NULL, const char* address = NULL, uint32_t baudrate = 0);
  /** Add serial address returns true does nothing */
  virtual bool addAddress(uint8_t destID, const char* address = NULL, uint16_t port = 0);
  /** Remove serial address returns true does nothing */
  virtual bool removeAddress(uint8_t destID);
  /**
  Sends txData using its length of bytes through the serial connection. Connection is broadcast
  destID is only used for packing / unpacking. Return false if no proper connection is establish
  */
  virtual bool send(uint8_t destID, uint8_t* txData, uint32_t txLength);
  /** Sets recieved data to rxData and sets the length of the data to rxLength
  Returns false if not aviable connection, No data is recieved, or time out*/
  virtual bool recv(uint8_t* rxData, uint32_t* rxLength);
private:
  Serial* local;
  /**
    Connections.
   */
  List < SerialConn* > * connections; 
};
} // namespace Network
} // namespace Comnet
#endif // __SERIAL_LINK_H