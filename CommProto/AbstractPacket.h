/* NOTE: This is only a rough implementation. Not the real Thing!! */
#ifndef __ABSTRACT_PACKET_H
#define __ABSTRACT_PACKET_H

#include <CommProto/CommsPacket.h>

/** Use this AbstractPacket when it comes to packing, sending , and receiving */
class AbstractPacket {
protected:
  PacketType packType;
  
public:
  AbstractPacket() { }
  virtual ~AbstractPacket() { }
};
#endif // __ABSTRACT_PACKET_H
