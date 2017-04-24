/* 
  UDP implementation for x-platform systems.

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
#include <CommProto/network/udp.h>
#include <cctype>

namespace comnet {
namespace network {


/***********************************************/
/******************* Private *******************/
/***********************************************/


bool UDP::UdpOpen(int* fd)
{
  bool result = true;
  initializeSockAPI(result);
  /** attempts to open socket 
      returns false if fails*/
  if ((*fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		debug::Log::Message(debug::LOG_ERROR, "socket() failed\n");
    result = false;
  }
  struct timeval tv;
  // 1 s timeout.
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  if (result && setsockopt(*fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv)) == 0) {
		debug::Log::Message(debug::LOG_NOTIFY, "Successful socket option.\n");
  } else {
		LOG_PRINTF(debug::LOG_ERROR, "Timeout not set!\nerrno=%d", GET_LAST_ERROR);
    result = false;
  }
  
  return result;
}


/***********************************************/
/******************* Public  *******************/
/***********************************************/
UDP::UDP() 
{
  //visual studio 2013 is having issues init array to 0
  //error is "cannot specify initializer of arrays knwon issues with 2013
  //make sure is node connected is set to false
  // for (int index = 0; index < MAX_CONNECTIONS; index++) {
  //   conn[index].socket_status = SOCKET_OPEN;
  // }
//  connected = false;
  sockaddr.socket_status = SOCKET_CLOSED;
}


UDP::UDP(UDP&& udp)
{
  std::swap(udp.sockaddr, sockaddr);
  fd = udp.fd;
  slen = udp.slen;
  udp.sockaddr.socket_status = SOCKET_CLOSED;
}


UDP& UDP::operator=(UDP&& udp)
{
  std::swap(udp.sockaddr, sockaddr);
  fd = udp.fd;
  slen = udp.slen;
  udp.sockaddr.socket_status = SOCKET_CLOSED;
  return *this;
}


UDP::~UDP()
{   
  
}


bool UDP::InitConnection(const char* port, const char* address)
{	
  //open socket and  check if socket is not already connected
  if (sockaddr.socket_status != SOCKET_OPEN && UdpOpen(&fd)) {
    uint16_t length = 0;
    sockaddr.port = std::atoi(port);
    sockaddr.id = 0; // Home
    sockaddr.socket = fd;
    str_length(port, length);
    // check if port in number
    for (int x = 0; x < length; x++) {
      if (!isdigit(port[x])) {
				debug::Log::Message(debug::LOG_NOTIFY, "initConnection 'port' argument is not a numerical digit for udp connection\n");
        return false;
      }
    }

    uint32_t portInt = std::atoi((char*)port);
          
      
    //setup address structure
    memset((char *)&sockaddr.socket_address, 0, sizeof(sockaddr.socket_address));
    sockaddr.socket_address.sin_family = AF_INET;
    sockaddr.socket_address.sin_port = htons(portInt);		//Converts to network byte order (big endian)
    sockaddr.socket_address.sin_addr.s_addr = inet_addr((char*)address);
      
    //bind socket
    if (bind(fd, (struct sockaddr *)&sockaddr.socket_address, sizeof(sockaddr.socket_address)) < 0) {
			debug::Log::Message(debug::LOG_ERROR, "bind failed");

      return false;
    }
      
    //set size of address
    slen = sizeof(sockaddr.socket_address);
    //set connected to true
    sockaddr.socket_status = SOCKET_OPEN;

    return true;
  }

	LOG_PRINTF(debug::LOG_ERROR, "Connection failed, error %d\n", GET_LAST_ERROR);
  //already connected or failed to open socket
  return false;
}


std::unique_ptr<UDP> UDP::Connect(uint8_t dest_id, const char* address, uint16_t port)
{
  uint16_t length = 0;
  str_length(address, length);
  UDP* udp = new UDP();
  if (udp->GetSocket().socket_status == SOCKET_CLOSED && length < ADDRESS_LENGTH) {
    //setup address structure
    memset((char *)&udp->GetSocket().socket_address, 0, sizeof(udp->GetSocket().socket_address));
    udp->GetSocket().socket_address.sin_family = AF_INET;
    udp->GetSocket().socket_address.sin_port = htons(port);
    udp->GetSocket().socket_address.sin_addr.s_addr = inet_addr((char *)address);
    udp->GetSocket().socket_status = SOCKET_CONNECTED;
    udp->GetSocket().id = dest_id;
    udp->GetSocket().port = port;
    udp->fd = fd;
    udp->slen = slen;
    return std::unique_ptr<UDP>(udp);
  }
  
  //already connected node or address is invalid
  return std::unique_ptr<UDP>(nullptr);	
}

bool UDP::Send(uint8_t* tx_data, uint32_t tx_length)
{
  if (sockaddr.socket_status == SOCKET_CONNECTED) {
    int slenSend = sizeof(sockaddr.socket_address);
    if (sendto(fd, (char *) tx_data, tx_length, 0, (struct sockaddr *) &sockaddr.socket_address, slen) < 0)
    {
      LOG_PRINTF(debug::LOG_NOTIFY, "sendto() failed. error=%d\n", GET_LAST_ERROR);
      return false;
    } else {	  						
			LOG_PRINTF(debug::LOG_NOTIFY, "\n**  Sent\t Length: %d, Port: %d, IP: %s **\n",
      tx_length, ntohs(sockaddr.socket_address.sin_port),
      inet_ntoa(sockaddr.socket_address.sin_addr));		  
    }
  }

  return true;
}


bool UDP::Recv(uint8_t* rx_data, uint32_t* rx_length)
{
  int length = 0;
  *rx_length = 0;
  /*
    TODO(Garcia): We need to set recvfrom() to nonblocking!
   */
  if (sockaddr.socket_status == SOCKET_CONNECTED) {
    length = recvfrom(fd, 
                      (char*)rx_data, MAX_BUFFER_SIZE, 
                      0, 
                      (struct sockaddr *) &sockaddr.socket_address, 
                      (socklen_t *) &slen);
  } else {
		debug::Log::Message(debug::LOG_ERROR, "UDP not connected can't receive\n");
    return false;//not connected
  }
  
  if (length < 0) return false;
    
	LOG_PRINTF(debug::LOG_NOTIFY, "\n**  Recieved\t Length: %d, Port: %d, IP: %s **\n",
      length, ntohs(sockaddr.socket_address.sin_port), inet_ntoa(sockaddr.socket_address.sin_addr));    
  *rx_length = (uint32_t)length;

  return true;
}

bool UDP::Close()
{
  if (sockaddr.socket_status != SOCKET_CLOSED) {
    closeSocket(fd);
    sockaddr.socket_status = SOCKET_CLOSED;
    return true;
  }
  return false;
}
} // namespace Network
} // namespace Comnet
