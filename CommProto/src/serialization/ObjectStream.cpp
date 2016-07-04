#include <CommProto/serialization/ObjectStream.h>

using namespace Comnet::Serialization;

ObjectStream::ObjectStream():bufferSize(0)
{
	streamBuffer = new uint8_t[STREAM_BUFFER_MAX_SIZE];	
}

ObjectStream::~ObjectStream()
{
	delete[] streamBuffer;
	streamBuffer = NULL;
}


//input
ObjectStream& ObjectStream::operator<<(uint8_t& data)
{
	packByte(data, streamBuffer);
	return *this;
}


//output
ObjectStream& ObjectStream::operator>>(uint8_t& data)
{
	data = unpackByte(streamBuffer);
	return *this;
}