/*
  Header. 

Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia.

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
#ifndef HEADERPACKET_H
#define HEADERPACKET_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/network/crc32.h>
#include <CommProto/architecture/api.h>

#define MAX_PACKET_SIZE 1024
#define KEY_LENGTH 16//AES::DEFAULT_KEYLENGTH and ENCRYPTION_BLOCK_BYTES should be the same
#define HEADER_SIZE 26 + KEY_LENGTH //26 + RANDOM iv SIZE which is the same as key length
#define MAX_BUFFER_SIZE MAX_PACKET_SIZE + HEADER_SIZE + CRC32_SIZE
#define TERMINAL_SEQUENCE_SIZE 3		
/** 
    Will specify what type of packet we are sending. This will ensure that PacketType is out there somewhere
    and that the user will specify the Class name. This will likely turn into a
 */
namespace comnet {


/** Packet header stcture for sending messages*/
class COMM_EXPORT Header {
public:
  uint8_t dest_id;
  uint8_t source_id;
  uint16_t msg_len;
  uint16_t msg_id;
  uint8_t source_time_arr[4];
  uint8_t iv[KEY_LENGTH];//random init vector for encryption

  int32_t GetSourceTime() const;

  void SetSourceTime(int32_t source_time);

  static uint32_t Serialize(Header& header, uint8_t* buffer, uint32_t offset);

  static uint32_t Deserialize(Header& header, uint8_t* buffer, uint32_t offset);
};
} // comnet
#endif//PACKET_H

