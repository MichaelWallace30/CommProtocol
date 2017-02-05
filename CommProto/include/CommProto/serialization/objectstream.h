/*
Serial configurations.

Copyright (C) 2016  Michael Wallace, Mario Garcia.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(At your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __OBJECT_STREAM_H
#define __OBJECT_STREAM_H
#include <CommProto/architecture/api.h>
#include <CommProto/serialization/marshal.h>
#include <CommProto/debug/comms_debug.h>
#include <CommProto/headerpacket.h>
#include <string>

namespace comnet {
namespace serialization {


/** 
  Object Stream Class will hold an array of data as a stream. 
  All data will be put into the stream with the overloaded operators '<<' 
  All data will be taken out of the stream with the overloaded operators '>>'
  The Stream is a stack Last In First Out (LIFO). 
  Therefore reverse the order data is put in when taking out.
  To input a char* buffer you can call setBuffer(buffer, length).
  Setting the buffer will erase information by setting current postion to new size of buffer.
  Inputing a buffer is used on the lower level when dealing directly with a data link.
*/
class COMM_EXPORT ObjectStream {

  COMM_DISALLOW_COPYING(ObjectStream);

private:
  /** Data stream to hold input as an array of bytes*/
  marshal_t stream_buffer;

  /** Current postion of streamBuffer and also size of streamBuffer*/
  int32_t curr_pos;
  /** Error Handingling methods*/
  void PrintErrorUnderFlow(){ COMMS_DEBUG("\nERROR:\t Source: ObjectStream\t Message: Buffer too small\n"); }
  /** Error Handingling methods*/
  void PrintErrorOverFlow(){ COMMS_DEBUG("\nERROR:\t Source: ObjectStream\t Message: Buffer full\n"); }

public:

  /** Header packet for communication*/
  Header header_packet;

  /** Constructor*/
  ObjectStream();

  // Move constructions.
  ObjectStream(ObjectStream&& obj)
    : stream_buffer(nullptr)
    , curr_pos(0)
    , header_packet(Header()) { 
    std::swap(stream_buffer, obj.stream_buffer);
    std::swap(curr_pos, obj.curr_pos);
    std::swap(header_packet, obj.header_packet);
  }


  ObjectStream& operator=(ObjectStream&& obj) {
    std::swap(obj.stream_buffer, stream_buffer);
    std::swap(obj.curr_pos, curr_pos);
    std::swap(obj.header_packet, header_packet);
    return *this;
  } 
  /** Destructor*/
  ~ObjectStream();
  /** getPostion returns the postion of the stream buffer*/
  int32_t GetPosition() const { return curr_pos; }
  /** getSize return the size of the current buffer*/
  int32_t GetSize() const { return curr_pos; }
  /** getBuffer returns a pionter to the buffer stream*/
  marshal_t GetBuffer() const { return stream_buffer; }
  
  Header& GetHeaderPacket() { return header_packet; } 
  /** setBuffer will take a pointer to an array of char and its len to become the new stream of data*/
  void SetBuffer(const char* buffer, int len);

  void SetHeader(Header& header);
  
  void SerializeHeader();
  Header DeserializeHeader();
  
  
  /** Overloaded input stream operators to put variables into the object stream*/  
  ObjectStream& operator<<(string_t& data);
  ObjectStream& operator<<(std::wstring& data);
  ObjectStream& operator<<(std::string& data);
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
  

  /** OVerloaded output stream operators to output variables to a variable from the object stream
    string_t (char*) must use malloc when inputing data into new c-string variable*/  
  ObjectStream& operator>>(string_t& data);
  ObjectStream& operator>>(std::wstring& data);
  ObjectStream& operator>>(std::string& data);
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


// Defines simplicity for calling objectstream.
#define OBJECTSTREAM comnet::serialization::ObjectStream
#define REF_OBJECTSTREAM comnet::serialization::ObjectStream&
} // Serialization namespace
} // Comnet namespace

#endif // __OBJECT_STREAM_H
