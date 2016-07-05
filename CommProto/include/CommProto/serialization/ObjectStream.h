#ifndef __OBJECT_STREAM_H
#define __OBJECT_STREAM_H

#include <CommProto/tools/data_structures/interface/InterfaceList.h>
#include <CommProto/serialization/Marshal.h>

namespace Comnet {
namespace Serialization {
/**
*/

#define STREAM_BUFFER_MAX_SIZE 1014
class ObjectStream {
private:	
	marshall_t streamBuffer;
	uint32_t currentPostion;

public:
  ObjectStream();
  ~ObjectStream();
  
  int32_t getSize(){ return currentPostion; }
  
  /** Overloaded input stream operators to put variables into the object stream*/
  
  ObjectStream& operator<<(string_t& data);
  ObjectStream& operator<<(wideString_t& data);
  ObjectStream& operator<<(uint8_t& data);  
  ObjectStream& operator<<(int8_t& data);
  ObjectStream& operator<<(uint16_t& data);
  ObjectStream& operator<<(int16_t& data);  
  ObjectStream& operator<<(int32_t& data);
  ObjectStream& operator<<(uint32_t& data);
  ObjectStream& operator<<(int64_t& data);
  ObjectStream& operator<<(uint64_t& data);
  ObjectStream& operator<<(real32_t& data);
  ObjectStream& operator<<(real64_t& data);
  

  /** OVerloaded output stream operators to output variables to a variable from the object stream*/
  
  ObjectStream& operator>>(string_t& data);
  ObjectStream& operator>>(wideString_t& data);
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
  

};
} // Serialization namespace
} // Comnet namespace

#endif // __OBJECT_STREAM_H
