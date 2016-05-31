/* NOTE: This is only a rough implementation. Not the real Thing!! */
#ifndef __ABSTRACT_PACKET_H
#define __ABSTRACT_PACKET_H

#include <CommProto/CommsPacket.h>

namespace Comnet {


// This class needs to be created by us. An Object stream will need to be serializing and deserializing.
class ObjectStream;

/** 
    Use this AbstractPacket when it comes to packing, sending , and receiving
*/
class AbstractPacket {
public:
  /**
     Allows us to define the type of class we are working with.
  */
  AbstractPacket(const char* className);
  /**
     Make this abstract class polymorphic.
  */
  virtual ~AbstractPacket();

  uint32_t getId() { return typeId; }

  /**
     Packing should be defined by the user.
  */
  virtual void pack(ObjectStream& obj) = 0;
  /**
     Unpacking should be defined by the user.
  */
  virtual void unpack(ObjectStream& obj) = 0;

private:
  /**
     Disallow this constructor from being constructed.
  */
  AbstractPacket() { }
  /**
     This id defines the type of object.
  */
  uint32_t typeId;
};
} // Comnet namespace 
#endif // __ABSTRACT_PACKET_H
