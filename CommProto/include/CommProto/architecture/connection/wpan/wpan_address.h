/*
  WPAN address handle.

Application Programming interface tags.

Copyright (C) 2016  Mario Garcia.

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
#ifndef __WPAN_ADDRESS_H
#define __WPAN_ADDRESS_H

#include <CommProto/architecture/connection/xbee_serial.h>

COM_EXTERN_C

// address for all nodes.
#define WPAN_BROADCAST_ALL_NODES               0xFFFF
// address for anti sleeping devs
#define WPAN_NOT_ASLEEP                        0xFFFD
// address for all routers and coordinators.
#define WPAN_BROADCAST_ROUTERS                 0xFFFC
// indicates 64-bit addressing
#define WPAN_UNDEFINED                         0xFFFE
// network coordinator using network address of 0x0000
#define WPAN_COORDINATOR                       0x0000


// 64-bit IEEE address. 
typedef union {
  uint8_t  b[8];
  uint16_t u[4];
  uint32_t i[2];
} address64;

#ifndef ADDR64_FORMAT_SEPARATOR
 #define ADDR64_FORMAT_SEPARATOR               '-'
#endif 

#define ADDR64_STRING_LEN                      (8 * 2 + 7 + 1)

char* format_address64( char* buffer, const address64* address);
bool_t address64_is_equal(const address64* addr1, const address64 addr2); 
bool_t address_64_is_zero(const address64* addr);
int32_t address64_parse(address64* addr, const char* str);


// 802.15.4 64bit IEEE address. Contains a 16 bit network address as well.
typedef struct wpan_address_t {
  address64 ieee;
  uint16_t  network;
} wpan_address_t;



END_EXTERN_C

#endif // __WPAN_ADDRESS_H