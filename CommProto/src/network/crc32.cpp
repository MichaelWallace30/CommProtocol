/*
  Cyclic Redunancy Check algorithm.

  Copyright (C) 2016  Michael Wallace, Alex Craig.

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
#include <CommProto/network/crc32.h>

#include <CommProto/debug/log.h>

namespace comnet {
namespace network {
/** CRC32 checksum function*/
unsigned int Crc32(unsigned char *message, uint32_t length) {
 int i, j;
 unsigned int byte, crc, mask;
 static unsigned int table[256];

 /* Set up the table, if necessary. */

 if (table[1] == 0) {
  for (byte = 0; byte <= 255; byte++) {
   crc = byte;
   for (j = 7; j >= 0; j--) {    // Do eight times.
    mask = -(crc & 1);
    crc = (crc >> 1) ^ (0xEDB88320 & mask);
   }
   table[byte] = crc;
  }
 }

 /* Through with table setup, now calculate the CRC. */
 i = 0;
 crc = 0xFFFFFFFF;
 while (length--) {
  byte = message[i];
  crc = (crc >> 8) ^ table[(crc ^ byte) & 0xFF];
  i = i + 1;
 }
 return ~crc;
}


void AppendCrc32(uint8_t* buffer, uint32_t *length){
   
 unsigned int crc = Crc32(buffer,*length);//calculate crc32
 unsigned char a = (crc >> 24) & 0xff;//leftmost
 unsigned char b = (crc >> 16) & 0xff;//next byte
 unsigned char c = (crc >> 8) & 0xff;//next byte
 unsigned char d = (crc)& 0xff;//right most

 #ifdef LITTLE_ENDIAN_COMNET
 //swap outter two
 unsigned char e = a;
 a = d;
 d = e;
 //swap middle two
 e = b;
 b = c;
 c = e;
 #endif

 //add crc32
 buffer[(*length)++] = a;
 buffer[(*length)++] = b;
 buffer[(*length)++] = c;
 buffer[(*length)++] = d;
   
}

COMM_EXPORT void Crc32ToArr(unsigned char * buffer, uint32_t length, uint8_t* crcBuffer)
{
  unsigned int crc = Crc32(buffer, length);//calculate crc32
  unsigned char a = (crc >> 24) & 0xff;//leftmost
  unsigned char b = (crc >> 16) & 0xff;//next byte
  unsigned char c = (crc >> 8) & 0xff;//next byte
  unsigned char d = (crc) & 0xff;//right most

#ifdef LITTLE_ENDIAN_COMNET
                                 //swap outter two
  unsigned char e = a;
  a = d;
  d = e;
  //swap middle two
  e = b;
  b = c;
  c = e;
#endif

  //add crc32
  crcBuffer[0] = a;
  crcBuffer[1] = b;
  crcBuffer[2] = c;
  crcBuffer[3] = d;
}

unsigned int TruncateCrc32(uint8_t* buffer, uint32_t *length){
 if (*length < 4) {
   return -1;
 }
 debug::Log::Message(debug::LOG_DEBUG, "Truncating CRC.\n");
 unsigned char a = buffer[--(*length)];
 unsigned char b = buffer[--(*length)];
 unsigned char c = buffer[--(*length)];
 unsigned char d = buffer[--(*length)];
#ifdef LITTLE_ENDIAN_COMNET
 //swap outter two
 unsigned char e = a;
 a = d;
 d = e;
 //swap middle two
 e = b;
 b = c;
 c = e;
 debug::Log::Message(debug::LOG_DEBUG, "Finished swapping abcd.\n");
#endif

 //store bytes into crc_recv
 unsigned int crc_recv = 0;
 ((char*)&crc_recv)[0] = a;
 ((char*)&crc_recv)[1] = b;
 ((char*)&crc_recv)[2] = c;
 ((char*)&crc_recv)[3] = d;
 debug::Log::Message(debug::LOG_DEBUG, "truncated CRC...\n");
 //return results
 return crc_recv;
 
}
}//End NETWORK namespace
}//End COMNET namespace
