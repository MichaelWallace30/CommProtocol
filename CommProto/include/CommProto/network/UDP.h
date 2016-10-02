/* 
  UDP configurations.

  Copyright (C) 2016  Michael Wallace, Mario Garcia.

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
#ifndef UDP_H
#define UDP_H

/* User Includes */
#include <CommProto/architecture/connection/socket-config.h>
#include <CommProto/headerpacket.h>
#include <CommProto/architecture/macros.h>//str_length(char*,int)
#include <stdio.h>//printf
#include <stdlib.h>//atoi
#include <CommProto/debug/comms_debug.h>

#define ADDRESS_LENGTH 16
#define MAX_CONNECTIONS 32

namespace comnet {
namespace network {

/**
UDP class
*/
class UDP {
private:  
  /** Array of connections: address, port, and if connceted*/
  socket_t conn[MAX_CONNECTIONS];
  
  /** Socket id (socket descriptor returned by socket)*/
  int fd;	
  /** Local address & port */
  //struct sockaddr_in sockaddr;
  socket_t sockaddr;
  /** recieved address & port */
  //struct sockaddr_in si_other;
  socket_t si_other;
  /** Local address length */
  int slen;
  
  /** message buffer */
  uint8_t rx_buf[MAX_BUFFER_SIZE];	
  
  /** Is local socket connected */
//  bool connected;
  
  /** Opens udp socket returns false if socket open fails*/
  bool udp_open(int* fd);
  
  
 public:
  
  /**Constuctor*/
  UDP();
  ~UDP();
  
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
  
};
} // namespace Network
} // namespace Comnet
#endif
