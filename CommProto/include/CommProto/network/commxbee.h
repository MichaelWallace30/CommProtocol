/*
  XBee class for handling xbee connections.

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
#ifndef __COMMXBEE_H
#define __COMMXBEE_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/connection/serial-config.h>


#include <map>

// Declarations from the xbee.h file of libxbee3.lib
struct xbee;
struct xbee_pkt;
struct xbee_con;
struct xbee_conAddress;

namespace comnet {
namespace network {
#define MAX_XBEE_PACKET_SIZE 72
/**
   XBee communications module. Used for any Xbee protocol connection made to 
   other devices. Still needs work.
*/
class COMM_EXPORT CommXBee {
public:
  /**
    Constructor that autoinitializes when created. function initialize is automatically called from 
    this constructor. 
   */
  CommXBee();

  /**
     Default destructor.
   */
  ~CommXBee();
  /**
    Initialize the port on this device.
   */
  bool Initialize(const char* port, speed_t baudrate);
  /**
  Adds an address to the link.
  */
  bool AddAddress(uint8_t destId, const char* address64Hex, uint16_t port = 0);
  /**
  Remove an address from the link.
  */
  bool RemoveAddress(uint8_t destId);
  /**
    Send data over to the destination node.
   */
  bool Send(uint8_t destId, uint8_t* txData, uint32_t txLength);
  /**
    Check Receive data.
   */
  bool Recv(uint8_t* rxData, uint32_t& rxLength);
 
private:
 uint8_t hexCharToInt(unsigned char value);
 uint8_t doubleHexCharToInt(unsigned char c1, unsigned char c2);
 void stringToAddress(const char* str, uint8_t length, struct xbee_conAddress &address);

 //unordered map of *con

 struct xbee *xbee;
 struct xbee_pkt *pkt;

 std::map<uint8_t, struct xbee_con*> xbees;

 
 //struct xbee_conAddress *address;	
 //struct xbee_conSettings *settings;//for broadcast recieve probably wont use
 
 int ret;

 
 std::map<uint8_t, struct xbee_con*>::iterator itXbeesRecv;

};
} // namespace Network
} // namespace Comnet
#endif // __COMMXBEE_H
