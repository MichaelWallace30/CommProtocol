/* 
  Endianness for network byte order.

  Copyright (C) 2016  Michael Wallace.

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
#include <architecture/os/include_defines.h>

#if COM_TARGET_OS == COM_OS_WINDOWS

#else   //COM_TRAGET_OS == COM_OS_LINUX || COM_TARGET_OS == COM_OS_APPLE

#include <arpa/inet.h>

#endif

#if COM_TARGET_OS == COM_OS_WINDOWS
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#else

#endif

#define htonll(x) (
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
(((uint64_t)htonl(x)) << 32) + htonl((x) >> 32)
#endif
)
