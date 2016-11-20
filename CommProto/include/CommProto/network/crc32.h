/*
  CRC 32 checksum.

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
#ifndef CRC32
#define CRC32
#define CRC32_SIZE 4

#include <CommProto/serialization/marshal.h>//endianess

namespace comnet {
namespace network {


/** 
  CRC32 checksum function
*/
COMM_EXPORT unsigned int Crc32(unsigned char *message, uint32_t length);
//Now replaced by CrcToArr
COMM_EXPORT void AppendCrc32(uint8_t* buffer, uint32_t *length);
COMM_EXPORT void Crc32ToArr(unsigned char* message, uint32_t length, uint8_t* crcBuffer);
COMM_EXPORT unsigned int TruncateCrc32(uint8_t* buffer, uint32_t *length);

}//End NETWORK namespace
}//End COMNET namespace
#endif // CRC32
