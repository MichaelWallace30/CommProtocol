/*
  Link for UDP connections.

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
#ifndef __UDP_LINK_H
#define __UDP_LINK_H

#include <CommProto/network/commslink.h>
#include <CommProto/network/udp.h>


namespace comnet {
namespace network {

/**
  UDP link works UDP connections. IT should work UDP connections, instead of UDP by itself, however
  the design needs to be redone in the future.
*/
class UDPLink : public CommsLink {
public:
  /** Opens socket, assigns local address & port, binds socket, sets slen to length of address, sets is connected on scucces/
  Returns false if open socket or bind fails*/
  bool initConnection(const char* port = NULL, const char* address = NULL, uint32_t baudrate = 0);
  /** Adds Address & port to destID value of array of aviable connections
  Returns false if connection is already connected*/
  virtual bool addAddress(uint8_t destID, const char* address = NULL, uint16_t port = 0);
  /** Sets connection to not available
  Returns false is no connection is found*/
  virtual bool removeAddress(uint8_t destID);
  /** Sends txData using its length of bytes through the destID connection which is establish through add adress
  Return false if no proper connection is establish*/
  virtual bool send(uint8_t destID, uint8_t* txData, uint32_t txLength);
  /** Sets recieved data to rxData and sets the length of the data to rxLength
  Returns false if not aviable connection or no data is recieved*/
  virtual bool recv(uint8_t* rxData, uint32_t* rxLength);
private:
  UDP local;
};
} // namespace Network
} // namespace Comnet
#endif // 