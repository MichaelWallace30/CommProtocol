#ifndef __OBJECT_STREAM_H
#define __OBJECT_STREAM_H

#include <CommProto/tools/data_structures/interface/InterfaceList.h>
#include <CommProto/serialization/marshall.h>

namespace Comnet {
namespace Serialization {
  

/**
*/
class ObjectStream {
public:
  ObjectStream();
  ~ObjectStream();

  int32_t getSize();
  
  ObjectStream& operator<<(string_t& data);
  ObjectStream& operator<<(uint8& data);
  ObjectStream& operator<<(int8& data);
  ObjectStream& operator<<(uint16& data);
  ObjectStream& operator<<(int16& data);
  ObjectStream& operator<<(uint16& data);
  ObjectStream& operator<<(int32& data);
  ObjectStream& operator<<(uint32& data);
  ObjectStream& operator<<(int64& data);
  ObjectStream& operator<<(uint64& data);
  ObjectStream& operator<<(real32& data);
  ObjectStream& operator<<(real64& data);

  ObjectStream& operator>>(string_t& data);
  ObjectStream& operator>>(uint8& data);
  ObjectStream& operator>>(int8& data);
  ObjectStream& operator>>(uint16& data);
  ObjectStream& operator>>(int16& data);
  ObjectStream& operator>>(uint32& data);
  ObjectStream& operator>>(int32& data);
  ObjectStream& operator>>(uint64& data);
  ObjectStream& operator>>(int64& data);
  ObjectStream& operator>>(real32& data);
  ObjectStream& operator>>(real64& data);

private:
  Tools::DataStructures::Interface::List<unsigned char*>& stream;
};
} // Serialization namespace
} // Comnet namespace

#endif // __OBJECT_STREAM_H
