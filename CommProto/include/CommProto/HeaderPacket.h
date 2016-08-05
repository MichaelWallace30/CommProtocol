#ifndef HEADERPACKET_H
#define HEADERPACKET_H

#include <stdint.h>


#define MAX_PACKET_SIZE 1024
#define HEADER_SIZE 22//not great way to do this
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
};



// all platform packets could be declared or include from another file here
// all platform structs should include a serialize and deserialze function
// serialize function should return the size and buffer should pass by refernce
//
//ex:  uint16_t serialize(uint8_t buffer[MAX_PACKET_SIZE]);
//ex:  void deserialze(uint8_t buffer[MAX_PACKET_SIZE]);
//
//    yes or no?

#endif//PACKET_H
