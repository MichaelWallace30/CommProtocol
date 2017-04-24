/* 
  UDP configurations.

  Copyright (C) 2016  Michael Wallace, Mario Garcia, Alex Craig.

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
#ifndef UDP_H
#define UDP_H
/* User Includes */
#include <CommProto/architecture/connection/socket-config.h>
#include <CommProto/architecture/api.h>
#include <CommProto/headerpacket.h>
#include <CommProto/architecture/macros.h>//str_length(char*,int)
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <CommProto/debug/log.h>

#define ADDRESS_LENGTH 16
#define MAX_CONNECTIONS 32

namespace comnet {
namespace network {

/**
UDP class
*/
class COMM_EXPORT UDP {
  COMM_DISALLOW_COPYING(UDP);
private:  
  /** Array of connections: address, port, and if connceted*/
  //socket_t conn[MAX_CONNECTIONS];
  
  /** Socket id (socket descriptor returned by socket)*/
  int fd;	
  /** Local address & port */
  //struct sockaddr_in sockaddr;
  socket_t sockaddr;
  /** Local address length */
  int slen;
  
  /** Opens udp socket returns false if socket open fails*/
  bool UdpOpen(int* fd);
  
 public:
  
  /**Constuctor*/
  UDP();
  UDP(UDP&& udp);
  ~UDP();
  
  UDP& operator=(UDP&& udp);
  /** Opens socket, assigns local address & port, binds socket, sets slen to length of address, sets is connected on scucces/
      Returns false if open socket or bind fails*/
  bool InitConnection(const char* port = NULL, const char* address = NULL);
  /** Adds Address & port to dest_id value of array of aviable connections
      Returns false if connection is already connected*/
  std::unique_ptr<UDP> Connect(uint8_t dest_id, const char* address = NULL, uint16_t port = 0);
  /** Sets connection to not available
      Returns false is no connection is found*/
  //bool RemoveAddress(uint8_t dest_id);
  /** Sends txData using its length of bytes through the dest_id connection which is establish through add adress
      Return false if no proper connection is establish*/
  bool Send(uint8_t* tx_data, uint32_t tx_length);
  /** Sets recieved data to rxData and sets the length of the data to rxLength
      Returns false if not aviable connection or no data is recieved*/
  bool Recv(uint8_t* rx_data, uint32_t* rx_length);

  socket_t& GetSocket() { return sockaddr; }

  bool Close();
};
} // namespace Network
} // namespace Comnet
#endif
