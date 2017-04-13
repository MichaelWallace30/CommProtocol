/* 
  C++ Socket configuration.

  Copyright (C) 2016  Michael Wallace, Mario Garcia.

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
#ifndef __COMMSOCKET_H
#define __COMMSOCKET_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>

class sockaddr_in;

namespace comnet {
namespace network {

/**
packet_data_status determines current state of the data that is currently being read.
Any action done onto our packet, will notify us of what is currently happening.
*/
enum packet_data_status_t {
  PACKET_SUCCESSFUL,
  PACKET_NO_DATA,
  PACKET_MORE_DATA,
  PACKET_DISCONNECTED,
};

/**
  Basic Socket Interface to handle TCP and UDP sockets.
*/
_COMNET_PUBLIC_API_ 
 _COMNET_INTERFACE_  class COMM_EXPORT CommSocket {
public:
  virtual ~CommSocket() 
  { }

  virtual int32_t SockSend(const char* buffer, 
                            uint32_t len, 
                            const char* address = "", 
                            uint32_t port = 0) = 0;

  virtual packet_data_status_t SockReceive(const char* buffer,
                                      uint32_t len,
																																						uint32_t& size,
                                      const char* address = "", 
                                      uint32_t port = 0) = 0;
  
  virtual int32_t SockConnect(const char* address, uint32_t port) = 0;
  
  virtual int32_t SockAsyncConnect(const char* address, uint32_t port) = 0;

  virtual int32_t SockListen(const char* address, uint32_t port) = 0;

  virtual int32_t SockListen(uint32_t port) = 0;

		virtual CommSocket* SockAccept() = 0;

  virtual CommSocket* SockAccept(sockaddr_in& connectedAddress) = 0;

  virtual void SockClose() = 0;
};

CommSocket* CreateTcpSocket();
CommSocket* CreateUdpSocket();
} // Network namespace
} // Comnet namespace
#endif // __COMMSOCKET_H
