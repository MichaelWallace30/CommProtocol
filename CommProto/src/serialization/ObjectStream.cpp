#include <CommProto/serialization/ObjectStream.h>

using namespace Comnet::Serialization;

ObjectStream::ObjectStream():currentPostion(0)
{
	streamBuffer = new uint8_t[STREAM_BUFFER_MAX_SIZE];	
}

ObjectStream::~ObjectStream()
{
	delete[] streamBuffer;
	streamBuffer = NULL;
}

//input

ObjectStream& ObjectStream::operator<<(string_t& data)
{
	uint8_t strLen = 0;
	str_length(data, strLen);
	
	if (currentPostion + strLen + 2 < STREAM_BUFFER_MAX_SIZE)
	{
		size = currentPostion += packString(data, streamBuffer + currentPostion);
	}
	return *this;
}


ObjectStream& ObjectStream::operator<<(wideString_t& data)
{

	//needs string leng
	uint32_t strLen = 0;
	str_length(data, strLen);	
	if (currentPostion + (strLen * 2) + 3< STREAM_BUFFER_MAX_SIZE)
	{
		size = currentPostion += packWideString(data, streamBuffer + currentPostion);
	}
	return *this;
}


ObjectStream& ObjectStream::operator<<(uint8_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		size = currentPostion += packByte(data, streamBuffer + currentPostion);
	}
	return *this;
}


ObjectStream& ObjectStream::operator<<(int8_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		size = currentPostion += packByte(data, streamBuffer + currentPostion);
	}
	return *this;
}


ObjectStream& ObjectStream::operator<<(uint16_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		size = currentPostion += packUint16(data, streamBuffer + currentPostion);
	}
	return *this;
}

ObjectStream& ObjectStream::operator<<(int16_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		size = currentPostion += packInt16(data, streamBuffer + currentPostion);
	}
	return *this;
}

ObjectStream& ObjectStream::operator<<(int32_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		size = currentPostion += packInt32(data, streamBuffer + currentPostion);
	}
	return *this;
}


ObjectStream& ObjectStream::operator<<(uint32_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		size = currentPostion += packUint32(data, streamBuffer + currentPostion);
	}
	return *this;
}
ObjectStream& ObjectStream::operator<<(int64_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		size = currentPostion += packInt64(data, streamBuffer + currentPostion);
	}
	return *this;
}
ObjectStream& ObjectStream::operator<<(uint64_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		size = currentPostion += packUint64(data, streamBuffer + currentPostion);
	}
	return *this;
}
ObjectStream& ObjectStream::operator<<(real32_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		size = currentPostion += packReal32(data, streamBuffer + currentPostion);
	}
	return *this;
}

ObjectStream& ObjectStream::operator<<(real64_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		size = currentPostion += packReal64(data, streamBuffer + currentPostion);
	}
	return *this;
}



//output
ObjectStream& ObjectStream::operator>>(string_t& data)
{
	uint32_t strLen = 0;//this is dangerous I don't know a soutlion

	strLen = unpackString(data,streamBuffer);

	currentPostion += strLen + 2;
		 
	return *this;
}

ObjectStream& ObjectStream::operator>>(wideString_t& data)
{
	uint32_t strLen = 0;//this is dangerous I don't know a soultion

	strLen = unpackWideString(data, streamBuffer);
	currentPostion += strLen + 3;
	
	return *this;
}

ObjectStream& ObjectStream::operator>>(uint8_t& data)
{
	if ((currentPostion -= sizeof(data)) >= 0)
	{
		data = unpackByte(streamBuffer + currentPostion);
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(int8_t& data)
{
	if ((currentPostion -= sizeof(data)) >= 0)
	{
		data = unpackByte(streamBuffer + currentPostion);
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(uint16_t& data)
{
	if ((currentPostion -= sizeof(data)) >= 0)
	{
		data = unpackUint16(streamBuffer + currentPostion);
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(int16_t& data)
{
	if ((currentPostion -= sizeof(data)) >= 0)
	{
		data = unpackInt16(streamBuffer + currentPostion);
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(uint32_t& data)
{
	if ((currentPostion -= sizeof(data)) >= 0)
	{
		data = unpackUint32(streamBuffer + currentPostion);
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(int32_t& data)
{
	if ((currentPostion -= sizeof(data)) >= 0)
	{
		data = unpackInt32(streamBuffer + currentPostion);
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(uint64_t& data)
{
	if ((currentPostion -= sizeof(data)) >= 0)
	{
		data = unpackUint64(streamBuffer + currentPostion);
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(int64_t& data)
{
	if ((currentPostion -= sizeof(data)) >= 0)
	{
		data = unpackInt64(streamBuffer + currentPostion);
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(real32_t& data)
{
	if ((currentPostion + sizeof(data)) <= getSize())
	{
		data = unpackReal32(streamBuffer + currentPostion);
		currentPostion += sizeof(data);
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(real64_t& data)
{
	if ((currentPostion + sizeof(data)) <= getSize())
	{
		data = unpackReal64(streamBuffer + currentPostion);
		currentPostion += sizeof(data);
	}
	return *this;
}
