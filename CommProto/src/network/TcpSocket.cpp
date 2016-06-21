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


#if COM_TARGET_OS != COM_OS_WINDOWS
 #include <netinet/tcp.h>
#endif

#define MAX_TICK 5

namespace Comnet {
namespace Network {

class TcpSocket : public CommSocket {
public:
  TcpSocket() {
    bool du = false;
    initializeSockAPI(du);
    mutex_init(&mutex);
    _socket.socket_status = SOCKET_CLOSED;
    _socket.socket = INVALID_SOCKET;
    _socket.port =  -1;
    _socket.id = -1;

  }

  int32_t sockConnect(const char* address, PORT port) {
    int32_t error = -1;
    if (_socket.socket != INVALID_SOCKET) {
      return error;
    }

    _socket.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (_socket.socket == INVALID_SOCKET) {
      comms_debug_log("Invalid socket..."); 
    } else {
      if (!setNonBlocking(_socket.socket, true)) {
          comms_debug_log("Failed to set socket to non blocking mode...");
        } else {
        struct sockaddr_in host = { 0 };
        initializeSockAddr(address, port, &host);

        _socket.socket_status = SOCKET_CONNECTING;
        int32_t connResult = connect(_socket.socket, (struct sockaddr*)&host, sizeof (host));
        if (connResult != INVALID_SOCKET || 
            (GET_LAST_ERROR != EWOULDBLOCK && 
            GET_LAST_ERROR != EINPROGRESS)) 
        {
          comms_debug_log("Cannot initiate connection");
        } else {
          time_t start_time, end_time;

          start_time = time(0);
          end_time = start_time;
          do {
            Sleep(500);
            int errLen = sizeof(error);
            if (getsockopt(_socket.socket, SOL_SOCKET, SO_ERROR, (char*)&error, (socklen_t*)&errLen) != 0) {
              comms_debug_log("error in getsockopt");
              break;
            } 
            if (error == 0) {
              comms_debug_log("Successful connection!");
              _socket.socket_status = SOCKET_CONNECTED;
              break;
            }
            end_time = time(0);
            printf("Timer: %d\n", (end_time - start_time));
          } while ((abs(end_time - start_time)) <= MAX_TICK);
        }
      }
    }

    return error;
  }

  int32_t sockSend(const char* buffer,
                    uint32_t len,
                    const char* address,
                    uint32_t port)
  {
    return 0;
  }

  packet_data_status_t sockReceive(const char* buffer,
                                   uint32_t len,
                                   const char* address,
                                   uint32_t port)
  {
    return PACKET_NO_DATA;
  }

  int32_t sockAsyncConnect(const char* address, uint32_t port) {
    return 0;
  }

  int32_t sockListen(const char* address, uint32_t port) {
    int32_t error = -1;
    if (_socket.socket_status != SOCKET_CLOSED) {
      comms_debug_log("Socket is not closed before listening...");
      return error;
    }

    _socket.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket.socket == INVALID_SOCKET) {
      comms_debug_log("Unsuccessful socket creation...");
    } else {
      struct sockaddr_in host;
      initializeSockAddr(address, port, &host);
      if (bind(_socket.socket, (struct sockaddr*)&host, sizeof(host)) < 0) {
        comms_debug_log("Failed to bind socket...");
      } else {
        if (listen(_socket.socket, SOMAXCONN) < 0) {
          comms_debug_log("Failed in listen function...");
        } else {
          if (!setNonBlocking(_socket.socket, true)) {
            comms_debug_log("Failed to set non blocking socket...");
          } else {
            comms_debug_log("Socket is successfully listening...");
            error = 0;
            _socket.socket_status = SOCKET_LISTENING;
            _socket.port = port;
          }
        }
      }
    }
    return error;
  }

  int32_t sockListen(uint32_t port) {
    return 0;
  }

  CommSocket* sockAccept() {
    if (_socket.socket_status != SOCKET_LISTENING) {
      comms_debug_log("socket is not listening...");
      return NULL;
    }
    
    CommSocket* tcpSock = NULL;
    SOCKET s = accept(_socket.socket, NULL, NULL);
    if (s == INVALID_SOCKET) {
      comms_debug_log("Failed to accept...");
    } else {
      setTcpNoDelay(s, true);
      tcpSock = new TcpSocket(s);
    }
    return tcpSock;
  }

  void sockDisconnect() {
  }

  void sockClose() {
    closeSocket(_socket.socket);
  }

protected:
  void initializeSockAddr(const char* address, PORT port, struct sockaddr_in* s) {
    s->sin_family = AF_INET;
    s->sin_port = htons(port);
    s->sin_addr.s_addr = inet_addr(address);
  }

  bool setNonBlocking(SOCKET sock, bool on) {
#ifdef _WIN32
    return ioctlsocket(sock, FIONBIO, (u_long*)&on) == 0;
#else
    int flags = fcntl(sock, F_GETFL, 0);
    return fcntl(sock, F_SETFL, on ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK)) == 0;
#endif
  }

  bool setTcpNoDelay(SOCKET sock, bool on) {
    return setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&on, sizeof(on)) == 0;
  }
private:
  TcpSocket(SOCKET socket) {
    bool du = false;
    _socket.socket_status = SOCKET_CONNECTED;
    _socket.socket = socket;
  }

  mutex_t mutex;
  
  socket_t _socket;
};

CommSocket* createTcpSocket() {
  return new TcpSocket();
}
} // Network namespace
} // Comnet namespace
