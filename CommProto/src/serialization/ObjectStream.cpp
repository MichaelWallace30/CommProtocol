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

	//needs string leng
	uint32_t strLen = 0;
	if (currentPostion + strLen < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packString(data, strLen, streamBuffer + currentPostion);
	}
	return *this;
}


ObjectStream& ObjectStream::operator<<(wideString_t& data)
{

	//needs string leng
	uint32_t strLen = 0;
	if (currentPostion + (strLen * 2) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packWideString(data, strLen, streamBuffer + currentPostion);
	}
	return *this;
}


ObjectStream& ObjectStream::operator<<(uint8_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packByte(data, streamBuffer + currentPostion);
	}
	return *this;
}


ObjectStream& ObjectStream::operator<<(int8_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packByte(data, streamBuffer + currentPostion);
	}
	return *this;
}


ObjectStream& ObjectStream::operator<<(uint16_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packUint16(data, streamBuffer + currentPostion);
	}
	return *this;
}

ObjectStream& ObjectStream::operator<<(int16_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packInt16(data, streamBuffer + currentPostion);
	}
	return *this;
}

ObjectStream& ObjectStream::operator<<(int32_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packInt32(data, streamBuffer + currentPostion);
	}
	return *this;
}


ObjectStream& ObjectStream::operator<<(uint32_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packUint32(data, streamBuffer + currentPostion);
	}
	return *this;
}
ObjectStream& ObjectStream::operator<<(int64_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packInt64(data, streamBuffer + currentPostion);
	}
	return *this;
}
ObjectStream& ObjectStream::operator<<(uint64_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packUint64(data, streamBuffer + currentPostion);
	}
	return *this;
}
ObjectStream& ObjectStream::operator<<(real32_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packReal32(data, streamBuffer + currentPostion);
	}
	return *this;
}

ObjectStream& ObjectStream::operator<<(real64_t& data)
{
	if (currentPostion + sizeof(data) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packReal64(data, streamBuffer + currentPostion);
	}
	return *this;
}



//output
ObjectStream& ObjectStream::operator>>(string_t& data)
{
	uint32_t strLen = 0;
	if ((currentPostion -= strLen) >= 0)
	{
		 unpackString(data, strLen,streamBuffer);
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(wideString_t& data)
{
	uint32_t strLen = 0;
	if ((currentPostion -= (strLen * 2)) >= 0)
	{
		unpackWideString(data, strLen, streamBuffer);
	}
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
	if ((currentPostion -= sizeof(data)) >= 0)
	{
		data = unpackReal32(streamBuffer + currentPostion);
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(real64_t& data)
{
	if ((currentPostion -= sizeof(data)) >= 0)
	{
		data = unpackReal64(streamBuffer + currentPostion);
	}
	return *this;
}
