/* 
  Socket configuration.

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
#ifndef __SOCKET_CONFIG_H
#define __SOCKET_CONFIG_H

#include <CommProto/architecture/os/include_defines.h>
#include <errno.h>

// Declare our libraries.
#if (COMMPROTO_TARGET_OS == COMMMPROTO_OS_WINDOWS)  
 #include <WinSock2.h>
 #include <WS2tcpip.h>
 #include <iphlpapi.h> 
 #pragma comment(lib ,"Ws2_32.lib")    

 #undef EWOULDBLOCK
 #define EWOULDBLOCK WSAEWOULDBLOCK
 #define s_addr  S_un.S_addr 
 #define closeSocket(x)	closesocket(x); WSACleanup()

 #define initializeWSAStartup(result) { \
           WSADATA wsa; \
           if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { \
             result = false; \
           } else { \
             result = true; \
           } \
         }
 #define initializeSockAPI(status) initializeWSAStartup(status)
 #ifndef SHUT_RDWR
  #define SHUT_RDWR SD_BOTH
 #endif
 #define GET_LAST_ERROR GetLastError()
#else
 #include <unistd.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <sys/time.h>
 #include <arpa/inet.h>
 #include <netdb.h>
 #include <fcntl.h>
 #include <stdint.h>
 #include <string.h>

 #define SOCKET_ERROR   -1
 #define INVALID_SOCKET -1
 typedef uint32_t SOCKET;   
 #define closeSocket(x)	close(x)
 #define initializeSockAPI(status) status = true;
 #define INVALID_SOCKET (SOCKET)(~0)
 #define GET_LAST_ERROR errno
 typedef unsigned short USHORT;
 typedef in_addr IN_ADDR;
#endif

typedef uint32_t PORT;
typedef uint32_t ID_T;
/**
   socket_status allows us to determine the current status of our sockets.
 */
enum socket_status_t {
  SOCKET_OPEN,
  SOCKET_CLOSED,
  SOCKET_RESOLVING,
  SOCKET_CONNECTING,
  SOCKET_CONNECTED,
  SOCKET_LISTENING,
  SOCKET_FAILED
};

/**
   This will be fixed, need to make this socket type generic for both windows and linux. 
   Both os'es use struct sockaddr_in, but they implement struct in_addr differently...
 */ 
struct socket_info {
  SOCKET socket;
  PORT   port;
  ID_T   id;

  socket_status_t socket_status;

  struct sockaddr_in socket_address;
  
};

typedef struct socket_info socket_t;

#endif // __SOCKET_CONFIG_H
