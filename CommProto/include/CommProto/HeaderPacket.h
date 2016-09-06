#ifndef HEADERPACKET_H
#define HEADERPACKET_H

#include <stdint.h>
#include <cstring>//memcpy

#define MAX_PACKET_SIZE 1024
#define HEADER_SIZE 22 //not great way to do this
#define ENCRYPTION_BLOCK_BYTES 16
#define KEY_LENGTH 16//AES::DEFAULT_KEYLENGTH
#define MAX_BUFFER_SIZE MAX_PACKET_SIZE + HEADER_SIZE
/** 
    Will specify what type of packet we are sending. This will ensure that PacketType is out there somewhere
    and that the user will specify the Class name. This will likely turn into a
 */

/** Packet header stcture for sending messages*/
struct header_t {
  uint8_t destID;
  uint8_t sourceID;
  uint16_t messageLength;
  uint16_t messageID;
  uint8_t IV[KEY_LENGTH];//random init vector for encryption

  uint32_t serialize(uint8_t* buffer, uint32_t offset){	  	  
	  memcpy(buffer + (offset++), &destID, 1);
	  memcpy(buffer + (offset++), &sourceID, 1);
	  memcpy(buffer + offset, &messageLength, 2);
	  offset += 2;
	  memcpy(buffer + offset, &messageID, 2);
	  offset += 2;
	  for (int x = 0; x < KEY_LENGTH; x++)
	  {
		  memcpy(buffer + (offset++), &IV[x], 1);
	  }
	  return offset;
  }

  uint32_t deserialize(uint8_t* buffer, uint32_t offset){
	  memcpy(&destID, buffer + (offset++), 1);
	  memcpy(&sourceID, buffer + (offset++), 1);
	  memcpy(&messageLength, buffer + offset, 2);
	  offset += 2;
	  memcpy(&messageID, buffer + offset, 2);
	  offset += 2;

	  for (int x = 0; x < KEY_LENGTH; x++)
	  {
		  memcpy(&IV[x], buffer + (offset++), 1);
	  }
	  return offset;
  }

};

#endif//PACKET_H
