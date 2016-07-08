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
	// + 1 for null termination + 1 for storing length of string as byte
	if (currentPostion + strLen + 2 < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packString(data,strLen, streamBuffer + currentPostion);
	}
	else
	{
		printErrorOverFlow();
	}
	return *this;
}


ObjectStream& ObjectStream::operator<<(wideString_t& data)
{

	//needs string leng
	uint32_t strLen = 0;
	str_length(data, strLen);	
	// + 2 for null termination + 1 for storing length of string as byte
	if (currentPostion + (strLen * 2) + 3< STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packWideString(data,strLen, streamBuffer + currentPostion);
	}
	else
	{
		printErrorOverFlow();
	}
	return *this;
}


ObjectStream& ObjectStream::operator<<(uint8_t& data)
{
	if (currentPostion + sizeof(uint8_t) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packByte(data, streamBuffer + currentPostion);
	}
	else
	{
		printErrorOverFlow();
	}
	return *this;
}


ObjectStream& ObjectStream::operator<<(int8_t& data)
{
	if (currentPostion + sizeof(int8_t) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packByte(data, streamBuffer + currentPostion);
	}
	else
	{
		printErrorOverFlow();
	}
	return *this;
}


ObjectStream& ObjectStream::operator<<(uint16_t& data)
{
	if (currentPostion + sizeof(uint16_t) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packUint16(data, streamBuffer + currentPostion);
	}
	else
	{
		printErrorOverFlow();
	}
	return *this;
}

ObjectStream& ObjectStream::operator<<(int16_t& data)
{
	if (currentPostion + sizeof(int16_t) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packInt16(data, streamBuffer + currentPostion);
	}
	else
	{
		printErrorOverFlow();
	}
	return *this;
}

ObjectStream& ObjectStream::operator<<(int32_t& data)
{
	if (currentPostion + sizeof(int32_t) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packInt32(data, streamBuffer + currentPostion);
	}
	else
	{
		printErrorOverFlow();
	}
	return *this;
}


ObjectStream& ObjectStream::operator<<(uint32_t& data)
{
	if (currentPostion + sizeof(uint32_t) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packUint32(data, streamBuffer + currentPostion);
	}
	else
	{
		printErrorOverFlow();
	}
	return *this;
}
ObjectStream& ObjectStream::operator<<(int64_t& data)
{
	if (currentPostion + sizeof(int64_t) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packInt64(data, streamBuffer + currentPostion);
	}
	else
	{
		printErrorOverFlow();
	}
	return *this;
}
ObjectStream& ObjectStream::operator<<(uint64_t& data)
{
	if (currentPostion + sizeof(uint64_t) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packUint64(data, streamBuffer + currentPostion);
	}
	else
	{
		printErrorOverFlow();
	}
	return *this;
}
ObjectStream& ObjectStream::operator<<(real32_t& data)
{
	if (currentPostion + sizeof(real32_t) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packReal32(data, streamBuffer + currentPostion);
	}
	else
	{
		printErrorOverFlow();
	}
	return *this;
}

ObjectStream& ObjectStream::operator<<(real64_t& data)
{
	if (currentPostion + sizeof(real64_t) < STREAM_BUFFER_MAX_SIZE)
	{
		currentPostion += packReal64(data, streamBuffer + currentPostion);
	}
	else
	{
		printErrorOverFlow();
	}
	return *this;
}



//output
ObjectStream& ObjectStream::operator>>(string_t& data)
{
	uint8_t strLen = 0;
	strLen = unpackByte(streamBuffer + currentPostion - 1);

	// + 1 for null termination + 1 for storing length of string as byte
	currentPostion -= (strLen + 2);

	strLen = unpackString(data,strLen,streamBuffer);

	
		 
	return *this;
}

ObjectStream& ObjectStream::operator>>(wideString_t& data)
{
	uint32_t strLen = 0;
	strLen = unpackByte(streamBuffer - currentPostion - 1);

	// + 2 for null termination + 1 for storing length of string as byte
	currentPostion -=( (strLen *2) + 3);
	strLen = unpackWideString(data, strLen,streamBuffer);
	
	
	return *this;
}

ObjectStream& ObjectStream::operator>>(uint8_t& data)
{
	if ((currentPostion -= sizeof(uint8_t)) >= 0)
	{
		data = unpackByte(streamBuffer + currentPostion);
	}
	else
	{
		currentPostion += sizeof(uint8_t);
		printErrorUnderFlow();
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(int8_t& data)
{
	if ((currentPostion -= sizeof(int8_t)) >= 0)
	{
		data = unpackByte(streamBuffer + currentPostion);
	}
	else
	{
		currentPostion += sizeof(int8_t);
		printErrorUnderFlow();
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(uint16_t& data)
{
	if ((currentPostion -= sizeof(uint16_t)) >= 0)
	{
		data = unpackUint16(streamBuffer + currentPostion);
	}
	else
	{
		currentPostion += sizeof(uint16_t);
		printErrorUnderFlow();
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(int16_t& data)
{
	if ((currentPostion -= sizeof(int16_t)) >= 0)
	{
		data = unpackInt16(streamBuffer + currentPostion);
	}
	else
	{
		currentPostion += sizeof(int16_t);
		printErrorUnderFlow();
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(uint32_t& data)
{
	if ((currentPostion -= sizeof(uint32_t)) >= 0)
	{
		data = unpackUint32(streamBuffer + currentPostion);
	}
	else
	{
		currentPostion += sizeof(uint32_t);
		printErrorUnderFlow();
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(int32_t& data)
{
	if ((currentPostion -= sizeof(int32_t)) >= 0)
	{
		data = unpackInt32(streamBuffer + currentPostion);
	}
	else
	{
		currentPostion += sizeof(int32_t);
		printErrorUnderFlow();
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(uint64_t& data)
{
	if ((currentPostion -= sizeof(uint64_t)) >= 0)
	{
		data = unpackUint64(streamBuffer + currentPostion);
	}
	else
	{
		currentPostion += sizeof(uint64_t);
		printErrorUnderFlow();
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(int64_t& data)
{
	if ((currentPostion -= sizeof(int64_t)) >= 0)
	{
		data = unpackInt64(streamBuffer + currentPostion);
	}
	else
	{
		currentPostion += sizeof(int64_t);
		printErrorUnderFlow();
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(real32_t& data)
{
	if ((currentPostion -= sizeof(real32_t)) >= 0)
	{
		data = unpackReal32(streamBuffer + currentPostion);		
	}
	else
	{
		currentPostion += sizeof(real32_t);
		printErrorUnderFlow();
	}
	return *this;
}

ObjectStream& ObjectStream::operator>>(real64_t& data)
{
	if ((currentPostion -= sizeof(real64_t)) >= 0)
	{
		data = unpackReal64(streamBuffer + currentPostion);		
	}
	else
	{
		currentPostion += sizeof(real64_t);
		printErrorUnderFlow();
	}
	return *this;
}
