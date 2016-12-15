 /*
  Link for UDP connections.

  Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia.

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
#ifndef __UDP_LINK_H
#define __UDP_LINK_H

#include <CommProto/network/commslink.h>
#include <CommProto/network/udp.h>
#include <map>
#include <memory>

namespace comnet {
namespace network {

/**
  UDP link works UDP connections. IT should work UDP connections, instead of UDP by itself, however
  the design needs to be redone in the future.
*/
class COMM_EXPORT UDPLink : public CommsLink {
public:
  // TODO(Garcia): UDP will need to have it's own list of udp objects to talk to.
  UDPLink() { protocol = UDP_LINK; }
  
  ~UDPLink() { }
  /** 
    Opens socket, assigns local address & port, binds socket, sets slen to length of address, sets is connected on scucces/
    Returns false if open socket or bind fails
  */
  bool InitConnection(const char* port = NULL, const char* address = NULL, uint32_t baudrate = 0) override;
  /** 
    Adds Address & port to dest_id value of array of aviable connections
    Returns false if connection is already connected
  */
  bool AddAddress(uint8_t dest_id, const char* address = NULL, uint16_t port = 0) override;
  /** 
    Sets connection to not available
    Returns false is no connection is found
  */
  bool RemoveAddress(uint8_t dest_id) override;
  /** 
    Sends txData using its length of bytes through the dest_id connection which is establish through add adress
    Return false if no proper connection is establish
  */
  bool Send(uint8_t dest_id, uint8_t* tx_data, uint32_t tx_length) override;
  /** 
    Sets recieved data to rxData and sets the length of the data to rxLength
    Returns false if not aviable connection or no data is recieved
  */
  bool Recv(uint8_t* rx_data, uint32_t* rx_length) override;

  void DigestCommand(const char* cmd) override { }
private:
  
  UDP local;
  /**
    Uses uint8_t key for mapping destination id to the UDP object.
  */
  std::map<uint8_t, std::unique_ptr<UDP>> clients;
};
} // namespace Network
} // namespace Comnet
#endif // 