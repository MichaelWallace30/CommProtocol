/*
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
#include <CommProto/headerpacket.h>

#include <cstring>

namespace comnet {


uint32_t Header::Serialize(uint8_t* buffer, uint32_t offset) {
  memcpy(buffer + (offset++), &dest_id, 1);
  memcpy(buffer + (offset++), &source_id, 1);
  memcpy(buffer + offset, &msg_len, 2);
  offset += 2;
  memcpy(buffer + offset, &msg_id, 2);
  offset += 2;
  for (int x = 0; x < KEY_LENGTH; x++){
    memcpy(buffer + (offset++), &iv[x], 1);
  }

  return offset;
}


uint32_t Header::Deserialize(uint8_t* buffer, uint32_t offset) {
  memcpy(&dest_id, buffer + (offset++), 1);
  memcpy(&source_id, buffer + (offset++), 1);
  memcpy(&msg_len, buffer + offset, 2);
  offset += 2;
  memcpy(&msg_id, buffer + offset, 2);
  offset += 2;
  
  for (int x = 0; x < KEY_LENGTH; x++) {
      memcpy(&iv[x], buffer + (offset++), 1);
  }

  return offset;  
}
} // comnet