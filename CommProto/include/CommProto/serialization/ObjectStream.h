#ifndef __OBJECT_STREAM_H
#define __OBJECT_STREAM_H

#include <CommProto/tools/data_structures/interface/InterfaceList.h>
#include <CommProto/serialization/Marshal.h>

namespace Comnet {
namespace Serialization {
  

/**
*/
class ObjectStream {
private:
	
	marshall_t streamBuffer;
public:
  ObjectStream();
  ~ObjectStream();
  
  int32_t getSize();
  
  ObjectStream& operator<<(string_t& data);
  ObjectStream& operator<<(uint8_t& data);
  ObjectStream& operator<<(int8_t& data);
  ObjectStream& operator<<(uint16_t& data);
  ObjectStream& operator<<(int16_t);  
  ObjectStream& operator<<(int32_t& data);
  ObjectStream& operator<<(uint32_t& data);
  ObjectStream& operator<<(int64_t& data);
  ObjectStream& operator<<(uint64_t& data);
  ObjectStream& operator<<(real32_t& data);
  ObjectStream& operator<<(real64_t& data);

  ObjectStream& operator>>(string_t& data);
  ObjectStream& operator>>(uint8_t& data);
  ObjectStream& operator>>(int8_t& data);
  ObjectStream& operator>>(uint16_t& data);
  ObjectStream& operator>>(int16_t& data);
  ObjectStream& operator>>(uint32_t& data);
  ObjectStream& operator>>(int32_t& data);
  ObjectStream& operator>>(uint64_t& data);
  ObjectStream& operator>>(int64_t& data);
  ObjectStream& operator>>(real32_t& data);
  ObjectStream& operator>>(real64_t& data);

private:
  //Tools::DataStructures::Interface::List<unsigned char*>& stream;
	//I cant figure out how to init this says default constructor is not allowed
};
} // Serialization namespace
} // Comnet namespace

#endif // __OBJECT_STREAM_H
