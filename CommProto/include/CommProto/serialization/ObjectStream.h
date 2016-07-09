#ifndef __OBJECT_STREAM_H
#define __OBJECT_STREAM_H

#include <CommProto/serialization/Marshal.h>
#include <CommProto/console/CommsDebug.h>
#include <string>

namespace Comnet {
namespace Serialization {
/**
*/

#define STREAM_BUFFER_MAX_SIZE 1014
class ObjectStream {
private:	
	marshall_t streamBuffer;
	int32_t currentPostion;
	void printErrorUnderFlow(){COMMS_DEBUG("ERROR:\t Source: ObjectStream\t Message: Buffer too small\n");}
	void printErrorOverFlow(){ COMMS_DEBUG("ERROR:\t Source: ObjectStream\t Message: Buffer full\n"); }

public:
  ObjectStream();
  ~ObjectStream();
  int32_t getPostion(){ return currentPostion;}
  void copyBuffer(char* buffer, int len);
  
  /** Overloaded input stream operators to put variables into the object stream*/
  
  ObjectStream& operator<<(string_t& data);
  ObjectStream& operator<<(std::wstring& data);
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
  ObjectStream& operator>>(std::wstring& data);
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
