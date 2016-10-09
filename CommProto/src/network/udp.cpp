/* 
  UDP implementation for x-platform systems.

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
#define _DEBUG 1
#include <CommProto/network/udp.h>
#include <ctype.h>

namespace comnet {
namespace network {


/***********************************************/
/******************* Private *******************/
/***********************************************/


bool UDP::UdpOpen(int* fd)
{
  bool result = false;
  initializeSockAPI(result);
  /** attempts to open socket 
      returns false if fails*/
  if ((*fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      COMMS_DEBUG("socket() failed\n");
      result = false;;
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
  for (int index = 0; index < MAX_CONNECTIONS; index++) {
    conn[index].socket_status = SOCKET_OPEN;
  }
//  connected = false;
  sockaddr.socket_status = SOCKET_OPEN;
}


UDP::~UDP()
{	
  closeSocket(fd);
}


bool UDP::InitConnection(const char* port, const char* address, uint32_t baudrate)
{	
  //open socket and  check if socket is not already connected
  if (sockaddr.socket_status != SOCKET_CONNECTED && UdpOpen(&fd)) {

    uint16_t length = 0;

    str_length(port, length);
    // check if port in number
    for (int x = 0; x < length; x++) {
      if (!isdigit(port[x])) {
        COMMS_DEBUG("initConnection 'port' argument is not a numerical digit for udp connection\n");

        return false;
      }
    }

    uint32_t portInt = atoi((char*)port);
	         
      
    //setup address structure
    memset((char *)&sockaddr.socket_address, 0, sizeof(sockaddr.socket_address));
    sockaddr.socket_address.sin_family = AF_INET;
    sockaddr.socket_address.sin_port = htons(portInt);
    sockaddr.socket_address.sin_addr.s_addr = inet_addr((char*)address);
      
    //bind socket
    if (bind(fd, (struct sockaddr *)&sockaddr.socket_address, sizeof(sockaddr.socket_address)) < 0) {
      COMMS_DEBUG("bind failed");

      return false;
    }
      
    //set size of address
    slen = sizeof(sockaddr.socket_address);
    //set connected to true
    sockaddr.socket_status = SOCKET_CONNECTED;

    return true;
  }

  COMMS_DEBUG("Connection failed, error %d\n", GET_LAST_ERROR);
  //already connected or failed to open socket
  return false;
}


bool UDP::AddAddress(uint8_t dest_id, const char* address, uint16_t port)
{
  uint16_t length = 0;
  str_length(address, length);
  if (conn[dest_id].socket_status == SOCKET_OPEN && length < ADDRESS_LENGTH) {
    //setup address structure
    memset((char *)&conn[dest_id].socket_address, 0, sizeof(conn[dest_id].socket_address));
    conn[dest_id].socket_address.sin_family = AF_INET;
    conn[dest_id].socket_address.sin_port = htons(port);
    conn[dest_id].socket_address.sin_addr.s_addr = inet_addr((char*)address);
    conn[dest_id].socket_status = SOCKET_CONNECTED;

    return true;
  }
  
  //already connected node or address is invalid
  return false;	
}


bool UDP::RemoveAddress(uint8_t dest_id)
{  
  if (conn[dest_id].socket_status == SOCKET_CONNECTED) {
    conn[dest_id].socket_status = SOCKET_OPEN;
    return true;
  }

  return false;
}


bool UDP::Send(uint8_t dest_id, uint8_t* tx_data, uint32_t tx_length)
{
  if (sockaddr.socket_status == SOCKET_CONNECTED) {
    int slenSend = sizeof(conn[dest_id].socket_address);
    if (sendto(fd, 
        (char *) tx_data, 
        tx_length, 0, 
        (struct sockaddr *) &conn[dest_id].socket_address, slen) < 0)
		{
      COMMS_DEBUG("sendto() failed\n");

      return false;
		} else {	  						
        COMMS_DEBUG("\n**  Sent\t Length: %d, Port: %d, IP: %s **\n", 
        tx_length, ntohs(conn[dest_id].socket_address.sin_port),
        inet_ntoa(conn[dest_id].socket_address.sin_addr));		  
		}
  }

  return false;
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
                      (struct sockaddr *) &si_other.socket_address, 
                      (socklen_t *) &slen);
  } else {
    COMMS_DEBUG("UDP not connected can't receive\n");
    return false;//not connected
  }
  
  if (length < 0) return false;
    
  COMMS_DEBUG("\n**  Recieved\t Length: %d, Port: %d, IP: %s **\n", length, ntohs(si_other.socket_address.sin_port) , inet_ntoa(si_other.socket_address.sin_addr));    
  *rx_length = (uint32_t)length;

  return true;
}
} // namespace Network
} // namespace Comnet
