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
#include <CommProto/architecture/os/comm_mutex.h>
#include <CommProto/serialization/marshal.h>
#include <cstring>

namespace comnet {


architecture::os::CommMutex header_mutex;


int32_t Header::GetSourceTime() const
{
  int32_t sourceTime = 0;
  for (int i = 0; i < 4; i++)
  {
    sourceTime |= (source_time_arr[i] & 0xff) << (8 * i);
  }
  return sourceTime;
}

void Header::SetSourceTime(int32_t sourceTime)
{
  for (int i = 0; i < 4; i++)
  {
    source_time_arr[i] = (sourceTime >> (8 * i)) & 0xff;
  }
}

uint32_t Header::Serialize(Header& header, uint8_t* buffer, uint32_t offset) {
  header_mutex.Lock();
  memcpy(buffer + (offset++), &header.dest_id, 1);
  memcpy(buffer + (offset++), &header.source_id, 1);
#ifndef LITTLE_ENDIAN_COMNET
  memcpy(buffer + offset, &header.msg_len, 2);
  offset += 2;
  memcpy(buffer + offset, &header.msg_id, 2);
  offset += 2;

  for (int i = 0; i < 4; i++)
  {
    memcpy(buffer + (offset++), &header.source_time_arr[i], 1);
  }
#else
  uint16_t networkOrderMsgLen = serialization::SwapEndianCopy(header.msg_len);
  memcpy(buffer + offset, &networkOrderMsgLen, 2);
  offset += 2;

  uint16_t networkOrderMsgID = serialization::SwapEndianCopy(header.msg_id);
  memcpy(buffer + offset, &networkOrderMsgID, 2);
  offset += 2;

  for (int i = 3; i >= 0; i--)
  {
    memcpy(buffer + (offset++), &header.source_time_arr[i], 1);
  }
#endif

  for (int x = 0; x < KEY_LENGTH; x++){
    memcpy(buffer + (offset++), &header.iv[x], 1);
  }
  header_mutex.Unlock();
  return offset;
}


uint32_t Header::Deserialize(Header& header, uint8_t* buffer, uint32_t offset) {
  header_mutex.Lock();
  memcpy(&header.dest_id, buffer + (offset++), 1);
  memcpy(&header.source_id, buffer + (offset++), 1);
#ifndef LITTLE_ENDIAN_COMNET
  memcpy(&header.msg_len, buffer + offset, 2);
  offset += 2;
  memcpy(&header.msg_id, buffer + offset, 2);
  offset += 2;
  for (int i = 0; i < 4; i++)
  {
    memcpy(&header.source_time_arr[i], buffer + (offset++), 2);
  }
#else
  memcpy(&header.msg_len, buffer + offset, 2);
  serialization::SwapEndian(header.msg_len);
  offset += 2;
  memcpy(&header.msg_id, buffer + offset, 2);
  serialization::SwapEndian(header.msg_id);
  offset += 2;

  for (int i = 3; i >= 0; i--)
  {
    memcpy(&header.source_time_arr[i], buffer + (offset++), 1);
  }
#endif
  for (int x = 0; x < KEY_LENGTH; x++) {
      memcpy(&header.iv[x], buffer + (offset++), 1);
  }
  header_mutex.Unlock();
  return offset;  
}
} // comnet