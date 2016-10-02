#ifndef HEADERPACKET_H
#define HEADERPACKET_H

#include <CommProto/architecture/os/include_defines.h>

#define MAX_PACKET_SIZE 1024
#define KEY_LENGTH 16//AES::DEFAULT_KEYLENGTH and ENCRYPTION_BLOCK_BYTES should be the same
#define HEADER_SIZE 22 + KEY_LENGTH //22 + RANDOM IV SIZE which is the same as key length
#define CRC32_SIZE 4
#define MAX_BUFFER_SIZE MAX_PACKET_SIZE + HEADER_SIZE + CRC32_SIZE
#define TERMINAL_SEQUENCE_SIZE 3		
/** 
    Will specify what type of packet we are sending. This will ensure that PacketType is out there somewhere
    and that the user will specify the Class name. This will likely turn into a
 */

/** Packet header stcture for sending messages*/
class header_t {
public:
  uint8_t destID;
  uint8_t sourceID;
  uint16_t messageLength;
  uint16_t messageID;
  uint8_t IV[KEY_LENGTH];//random init vector for encryption

  uint32_t serialize(uint8_t* buffer, uint32_t offset);

  uint32_t deserialize(uint8_t* buffer, uint32_t offset);
};

#endif//PACKET_H
