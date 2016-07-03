#include <CommProto/serialization/ObjectStream.h>

using namespace Comnet::Serialization;

ObjectStream::ObjectStream()
{
	streamBuffer = new uint8_t[100];
}

ObjectStream::~ObjectStream()
{
	delete streamBuffer;
}

ObjectStream& ObjectStream::operator<<(uint8_t& data)
{
	data = unpackByte(streamBuffer);
	return *this;
}

ObjectStream& ObjectStream::operator>>(uint8_t& data)
{
	packByte(data, streamBuffer);
	return *this;
}