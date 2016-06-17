/* 
  Socket configuration.

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
#ifndef __SOCKET_CONFIG_H
#define __SOCKET_CONFIG_H

#include <CommProto/architecture/os/include_defines.h>

// Declare our libraries.
#if (COM_TARGET_OS == COM_OS_WINDOWS)  
 #include <WinSock2.h>
 #include <WS2tcpip.h>
 #include <iphlpapi.h>
 #define WIN32_LEAN_AND_MEAN 
 #include <Windows.h>  
 #pragma comment(lib ,"Ws2_32.lib")    
 #define s_addr  S_un.S_addr 
 #define closSocket(x)	closesocket(x); WSACleanup()
#else
 #include <unistd.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <arpa/inet.h>
 #include <netdb.h>
 #include <fcntl.h>
 #include <stdint.h>
 #include <string.h>

 #define SOCKET_ERROR   -1
 #define INVALID_SOCKET -1
 typedef uint32_t SOCKET;   
 #define closSocket(x)	close(x)
#endif

typedef uint32_t PORT;
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
  SOCKET_FAILED,
  SOCKET_UNINITIALIZED,
};

/**
   packet_data_status determines current state of the data that is currently being read.
   Any action done onto our packet, will notify us of what is currently happening.
 */
enum packet_data_status_t {
  PACKET_SUCCESSFUL,
  PACKET_NO_DATA,
  PACKET_READING_DATA,
  PACKET_DISCONNECTED,
};

/**
   This will be fixed, need to make this socket type generic for both windows and linux. 
   Both os'es use struct sockaddr_in, but they implement struct in_addr differently...
 */ 
struct socket_info {
  SOCKET socket;
  PORT   port;

  socket_status_t socket_status;

  struct sockaddr_in socket_address;
  
};

typedef struct socket_info socket_t;

#endif // __SOCKET_CONFIG_H
