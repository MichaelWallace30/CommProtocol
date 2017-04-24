/*
XBee Link that handles all xbee connections.

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
#include <CommProto/network/commxbee.h>
//#include <CommProto/network/znodetable.h>
#include <CommProto/debug/log.h>
#include <CommProto/architecture/macros.h>
#include <CommProto/headerpacket.h>

#include <regex>
#include <cstring>
#include <ctype.h>
#include <xbee.h>

namespace comnet {
namespace network {

CommXBee::CommXBee()
{
 itXbeesRecv = xbees.begin();
}


CommXBee::~CommXBee()
{
 
}


// Initialize the xbee device.
bool CommXBee::Initialize(const char* port, speed_t baudrate) {

 char port_name[55];
 #if (COMMPROTO_TARGET_OS == COMMMPROTO_OS_WINDOWS)
  sprintf(port_name, "\\\\.\\%s", port);//change to com
 #else
  sprintf(port_name, "%s", port);
 #endif		
 if ((ret = xbee_setup(&xbee, "xbee5", port_name, baudrate)) != XBEE_ENONE) {		
  printf("Construct ret: %d (%s)\n", ret, xbee_errorToStr((xbee_err)ret));		
  return false;
 }	
 return true;
}


/**
Adds an address to the link.
*/
bool CommXBee::AddAddress(uint8_t destId, const char* address64Hex , uint16_t port)
{
 struct xbee_conAddress conAddress;
 memset(&conAddress, 0, sizeof(conAddress));
 stringToAddress(address64Hex, 16, conAddress);	
 struct xbee_con *con;
 //xbee_conAlloc((con));

 if ((ret = xbee_conNew(xbee, &con, "Data", &conAddress)) != XBEE_ENONE) {
  LOG_PRINTF(debug::LOG_WARNING, "xbee_conNew() node_id: %d  returned: %d", destId, ret);		
  return false;
 }

 xbees.insert({ destId, con });

 return true;
}
/**
Remove an address from the link.
*/
bool CommXBee::RemoveAddress(uint8_t destId)
{
 xbees.erase(destId);
 return false;
}
/**
Send data over to the destination node.
*/

/* This function iterates through all xbee connections,
and checks to if it all packets for that connection was received. i.e.
(seq/ maxSeq). It supposed to stay at the next iteration if it
returns true. So connection 5/10 return true it will not start at
connection 1 but stay at connection 6 for when it returns. If it gets to
10/10 and no packet was found i.e. pkt == null it should return false. */
bool CommXBee::Recv(uint8_t* rxData, uint32_t& rxLength) {
 
 rxLength = 0;
 //xbees iterator is set in constructor as begin
 //iterator should be left on last index + 1 of xbees when a message is recieved
 //and start on next index when this method is called agan

 unsigned char buffer[MAX_XBEE_PACKET_SIZE];

 while (itXbeesRecv != xbees.end())//loop through all connections
 {
  struct xbee_con *con = itXbeesRecv->second;
  xbee_conRx(con, &pkt, NULL);
  itXbeesRecv++;//next xbee connection
    
  uint8_t seq;
  uint8_t maxSeq;
  if (pkt != NULL) {
   do
   {
    // Package is not null, means we got a package from somewhere.
    if (pkt != NULL) {
     for (int i = 0; i < pkt->dataLen; i++) {
      buffer[i] = pkt->data[i];
     }
     int size = pkt->dataLen - 2;
     seq = buffer[0];
     maxSeq = buffer[1];
     rxLength += size;
     memcpy(&rxData[(seq - 1) * (MAX_XBEE_PACKET_SIZE - 2)], &buffer[2], size);

     if (xbee_pktFree(pkt) != XBEE_ENONE);					
    }
    if(seq< maxSeq)xbee_conRx(con, &pkt, NULL);
   } while (seq < maxSeq);
   return true;
  }
 }

 itXbeesRecv = xbees.begin();//start iterator from begging
 //no error to report just no message recv
 return false;
}

bool CommXBee::Send(uint8_t destId, uint8_t* txData, uint32_t txLength) {
 
 uint8_t seq = 1;
 uint8_t maxSeq;
 uint8_t offset = 0;

 unsigned char buffer[MAX_XBEE_PACKET_SIZE];

 if (txLength % (MAX_XBEE_PACKET_SIZE - 2) != 0)
 {
  maxSeq = (uint8_t)(txLength / (MAX_XBEE_PACKET_SIZE - 2)) + 1;
 }
 else
 {
  maxSeq = (uint8_t)(txLength / (MAX_XBEE_PACKET_SIZE - 2));
 }
 
 auto it = xbees.find(destId);

 if (it != xbees.end())
 {

  while (seq <= maxSeq)
  {
   buffer[0] = seq;
   buffer[1] = maxSeq;

   uint8_t size = (txLength - ((seq -1) *(MAX_XBEE_PACKET_SIZE - 2)));			
   if (size > (MAX_XBEE_PACKET_SIZE - 2)) size = MAX_XBEE_PACKET_SIZE - 2;			
   memcpy(&buffer[2], &txData[offset], size);
   offset += size;

   xbee_connTx(it->second, NULL, buffer, size + 2);
   LOG_PRINTF(debug::LOG_NOTE, "Xbee sent data to DestID: %d!\n", destId);
   seq++;
  }
  return true;
 }
 
 LOG_PRINTF(debug::LOG_ERROR, "DestID: %d on xbee not found!\n", destId);
 return false;
}


uint8_t CommXBee::hexCharToInt(unsigned char value)
{
 if (isalpha(value))
 {
  unsigned char c = toupper(value);
  return c = c - '7';// or c -= 55
 }
 else if (isdigit(value))
 {
  return value = value - '0';
 }
 return 0;
}

// 16 ^ 1 * 15 + 16 ^ 0 * 15
uint8_t CommXBee::doubleHexCharToInt(unsigned char c1, unsigned char c2)
{
 //16 ^ 1 * C1 + 16 ^ 0 * C2
 uint8_t value = hexCharToInt(c1) * 16;
 value += hexCharToInt(c2);
 return value;
}

void CommXBee::stringToAddress(const char* str, uint8_t length, xbee_conAddress &address)
{
 //if no spaces in string increment is 2 if spaces increment is 3
 int increment = (str[2] == ' ') ? 3 : 2;
 address.addr64_enabled = 1;
 int x = 0;
 //check if string is right size to calc double hex char to int
 if ((increment == 2 && (length) % increment == 0)
  || (increment == 3 && ((length - 1) % increment == 0)))
 {
  //loop for each pair of chars to find int value ex: FF = 255
  for (int index = 0; index < length; index += increment)
  {

   address.addr64[x] = doubleHexCharToInt(str[index], str[index + 1]);
   x++;
  }
 }
 else
 {
	 debug::Log::Message(debug::LOG_ERROR, "Invalid hex to string conversion.\n");
  return;
 }
}

} // namespace Network
} // namespace Comnet