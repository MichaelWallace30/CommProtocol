#include <CommProto/HeaderPacket.h>

#include <cstring>

uint32_t header_t::serialize(uint8_t* buffer, uint32_t offset) {
  memcpy(buffer + (offset++), &destID, 1);
  memcpy(buffer + (offset++), &sourceID, 1);
  memcpy(buffer + offset, &messageLength, 2);
  offset += 2;
  memcpy(buffer + offset, &messageID, 2);
  offset += 2;
  for (int x = 0; x < KEY_LENGTH; x++){
    memcpy(buffer + (offset++), &IV[x], 1);
  }

  return offset;
}


uint32_t header_t::deserialize(uint8_t* buffer, uint32_t offset) {
  memcpy(&destID, buffer + (offset++), 1);
  memcpy(&sourceID, buffer + (offset++), 1);
  memcpy(&messageLength, buffer + offset, 2);
  offset += 2;
  memcpy(&messageID, buffer + offset, 2);
  offset += 2;
  
  for (int x = 0; x < KEY_LENGTH; x++) {
      memcpy(&IV[x], buffer + (offset++), 1);
  }

  return offset;  
}
