/* 
  TCP/IP connections.

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
#include <CommProto/network/CommSocket.h>

#include <CommProto/console/CommsDebug.h>

#include <CommProto/architecture/os/os_threads.h>
#include <CommProto/architecture/os/os_mutex.h>
#include <CommProto/architecture/connection/socket-config.h>

#include <stdlib.h>
#include <ctime>

namespace Comnet {
namespace Network {

class TcpSocket : public CommSocket {
public:
  TcpSocket() {

    mutex_init(&mutex);
    _socket.socket_status = SOCKET_CLOSED;
    _socket.socket = INVALID_SOCKET;
    _socket.port =  -1;
    _socket.id = -1;

  }

  uint32_t sockConnect(const char* address, PORT port) {
    uint32_t error = -1;
    if (_socket.socket != INVALID_SOCKET) {
      return error;
    }
    
    
  }
private:
  mutex_t mutex;
  
  socket_t _socket;
};
} // Network namespace
} // Comnet namespace
