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
#include <CommProto/serialization/objectstream.h>
#include <CommProto/debug/log.h>

#include <string.h>

namespace comnet {
namespace serialization {

ObjectStream::ObjectStream():curr_pos(HEADER_SIZE)
{
 stream_buffer = new uint8_t[MAX_BUFFER_SIZE];
}

ObjectStream::~ObjectStream()
{
 delete[] stream_buffer;
 stream_buffer = NULL;
}


void ObjectStream::SetBuffer(const char* buffer, int len)
{
 for (curr_pos = 0; curr_pos < len; curr_pos++)
 {
  stream_buffer[curr_pos] = buffer[curr_pos];
 }
  DeserializeHeader();
}

void ObjectStream::SetHeader(Header& header)
{
  header_packet = header;
}

void ObjectStream::SerializeHeader()
{
  //not sure if more is needed kind of a waste of space
  Header::Serialize(header_packet, stream_buffer, 0);
}


Header ObjectStream::DeserializeHeader()
{
 Header header;
 Header::Deserialize(header, stream_buffer, 0);	
  header_packet = header;
 return header;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
/********************* INPUT ***************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/

ObjectStream& ObjectStream::operator<<(const string_t& data)
{
 uint8_t strLen = 0;
 str_length(data, strLen);
 // + 1 for null termination + 1 for storing length of string as byte
 if (curr_pos + strLen + 2 < MAX_PACKET_SIZE)
 {
  //LOG_PRINTF(debug::LOG_DEBUG, "STRING BEFORE: %d\n", curr_pos);
  curr_pos += PackString(data, strLen, stream_buffer + curr_pos);
  //LOG_PRINTF(debug::LOG_DEBUG, "STRING AFTER: %d\n", curr_pos);
 }
 else
 {
  PrintErrorOverFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator<<(const std::wstring& data)
{

 //needs string leng
 uint32_t strLen = data.length();
 // + 2 for null termination + 1 for storing length of string as byte
 if (curr_pos + strLen + 2 < MAX_PACKET_SIZE)
 {
   //LOG_PRINTF(debug::LOG_DEBUG, "SIZE BEFORE: %d\n", curr_pos);
   curr_pos += PackWideString(data,strLen, stream_buffer + curr_pos);
	 //LOG_PRINTF(debug::LOG_DEBUG, "SIZE AFTER: %d\n", curr_pos);
 }
 else
 {
  PrintErrorOverFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator<<(const std::string& data) 
{
  uint32_t strLen = data.size();
  if (curr_pos + strLen + 2 < MAX_PACKET_SIZE) {
    curr_pos += PackString((string_t)data.c_str(), strLen, stream_buffer + curr_pos);
  } else {
    PrintErrorOverFlow();
  }

  return *this;
}


ObjectStream& ObjectStream::operator<<(const uint8_t& data)
{
 if (curr_pos + sizeof(uint8_t) < MAX_PACKET_SIZE)
 {
  curr_pos += PackByte(data, stream_buffer + curr_pos);
 }
 else
 {
  PrintErrorOverFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator<<(const int8_t& data)
{
 if (curr_pos + sizeof(int8_t) < MAX_PACKET_SIZE)
 {
  curr_pos += PackByte(data, stream_buffer + curr_pos);
 }
 else
 {
  PrintErrorOverFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator<<(const uint16_t& data)
{
 if (curr_pos + sizeof(uint16_t) < MAX_PACKET_SIZE)
 {
  curr_pos += PackUint16(data, stream_buffer + curr_pos);
 }
 else
 {
  PrintErrorOverFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator<<(const int16_t& data)
{
 if (curr_pos + sizeof(int16_t) < MAX_PACKET_SIZE)
 {
  curr_pos += PackInt16(data, stream_buffer + curr_pos);
 }
 else
 {
  PrintErrorOverFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator<<(const int32_t& data)
{
 if (curr_pos + sizeof(int32_t) < MAX_PACKET_SIZE)
 {
  curr_pos += PackInt32(data, stream_buffer + curr_pos);
 }
 else
 {
  PrintErrorOverFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator<<(const uint32_t& data)
{
 if (curr_pos + sizeof(uint32_t) < MAX_PACKET_SIZE)
 {
  curr_pos += PackUint32(data, stream_buffer + curr_pos);
 }
 else
 {
  PrintErrorOverFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator<<(const int64_t& data)
{
 if (curr_pos + sizeof(int64_t) < MAX_PACKET_SIZE)
 {
  curr_pos += PackInt64(data, stream_buffer + curr_pos);
 }
 else
 {
  PrintErrorOverFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator<<(const uint64_t& data)
{
 if (curr_pos + sizeof(uint64_t) < MAX_PACKET_SIZE)
 {
  curr_pos += PackUint64(data, stream_buffer + curr_pos);
 }
 else
 {
  PrintErrorOverFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator<<(const real32_t& data)
{
 if (curr_pos + sizeof(real32_t) < MAX_PACKET_SIZE)
 {
  curr_pos += PackReal32(data, stream_buffer + curr_pos);
 }
 else
 {
  PrintErrorOverFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator<<(const real64_t& data)
{
 if (curr_pos + sizeof(real64_t) < MAX_PACKET_SIZE)
 {
  curr_pos += PackReal64(data, stream_buffer + curr_pos);
 }
 else
 {
  PrintErrorOverFlow();
 }
 return *this;
}

ObjectStream & comnet::serialization::ObjectStream::operator<<(const ObjSerializable & data)
{
	data.Input(*this);
	return *this;
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
/********************* OUTPUT **************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/


ObjectStream& ObjectStream::operator>>(string_t& data)
{
 uint8_t strLen = 0;
 strLen = UnpackByte(stream_buffer + curr_pos - 1);
 // + 1 for null termination + 1 for storing length of string as byte
 curr_pos -= (strLen + 2);

 strLen = UnpackString(data,strLen,stream_buffer + curr_pos);



 return *this;
}


ObjectStream& ObjectStream::operator>>(std::wstring& data)
{
 uint32_t strLen = UnpackByte(stream_buffer + curr_pos - 1);
 data.resize(strLen + 1);

 // + 2 for null termination + 1 for storing length of string as byte
 curr_pos -= (strLen + 2);
 strLen = UnpackWideString(data, strLen,stream_buffer + curr_pos);

 return *this;
}


ObjectStream& ObjectStream::operator>>(std::string& data) 
{
  uint32_t strLen = UnpackByte(stream_buffer + curr_pos - 1);
  data.resize(strLen + 1);
  curr_pos -= (strLen + 2);
  string_t temp = (string_t) malloc(data.size());
  strLen = UnpackString(temp, strLen, stream_buffer + curr_pos);
  data = temp;
  free_pointer(temp);
  return *this;
}


ObjectStream& ObjectStream::operator>>(uint8_t& data)
{
 if ((curr_pos -= sizeof(uint8_t)) >= HEADER_SIZE)
 {
  data = UnpackByte(stream_buffer + curr_pos);
 }
 else
 {
  curr_pos += sizeof(uint8_t);
  PrintErrorUnderFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator>>(int8_t& data)
{
 if ((curr_pos -= sizeof(int8_t)) >= HEADER_SIZE)
 {
  data = UnpackByte(stream_buffer + curr_pos);
 }
 else
 {
  curr_pos += sizeof(int8_t);
  PrintErrorUnderFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator>>(uint16_t& data)
{
 if ((curr_pos -= sizeof(uint16_t)) >= HEADER_SIZE)
 {
  data = UnpackUint16(stream_buffer + curr_pos);
 }
 else
 {
  curr_pos += sizeof(uint16_t);
  PrintErrorUnderFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator>>(int16_t& data)
{
 if ((curr_pos -= sizeof(int16_t)) >= HEADER_SIZE)
 {
  data = UnpackInt16(stream_buffer + curr_pos);
 }
 else
 {
  curr_pos += sizeof(int16_t);
  PrintErrorUnderFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator>>(uint32_t& data)
{
 if ((curr_pos -= sizeof(uint32_t)) >= HEADER_SIZE)
 {
  data = UnpackUint32(stream_buffer + curr_pos);
 }
 else
 {
  curr_pos += sizeof(uint32_t);
  PrintErrorUnderFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator>>(int32_t& data)
{
 if ((curr_pos -= sizeof(int32_t)) >= HEADER_SIZE)
 {
  data = UnpackInt32(stream_buffer + curr_pos);
 }
 else
 {
  curr_pos += sizeof(int32_t);
  PrintErrorUnderFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator>>(uint64_t& data)
{
 if ((curr_pos -= sizeof(uint64_t)) >= HEADER_SIZE)
 {
  data = UnpackUint64(stream_buffer + curr_pos);
 }
 else
 {
  curr_pos += sizeof(uint64_t);
  PrintErrorUnderFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator>>(int64_t& data)
{
 if ((curr_pos -= sizeof(int64_t)) >= HEADER_SIZE)
 {
  data = UnpackInt64(stream_buffer + curr_pos);
 }
 else
 {
  curr_pos += sizeof(int64_t);
  PrintErrorUnderFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator>>(real32_t& data)
{
 if ((curr_pos -= sizeof(real32_t)) >= HEADER_SIZE)
 {
  data = UnpackReal32(stream_buffer + curr_pos);
 }
 else
 {
  curr_pos += sizeof(real32_t);
  PrintErrorUnderFlow();
 }
 return *this;
}


ObjectStream& ObjectStream::operator>>(real64_t& data)
{
 if ((curr_pos -= sizeof(real64_t)) >= HEADER_SIZE)
 {
  data = UnpackReal64(stream_buffer + curr_pos);
 }
 else
 {
  curr_pos += sizeof(real64_t);
  PrintErrorUnderFlow();
 }
 return *this;
}


ObjectStream & comnet::serialization::ObjectStream::operator>>(ObjSerializable & data)
{
	data.Output(*this);
	return *this;
}
} // serialization.
} // comnet.
