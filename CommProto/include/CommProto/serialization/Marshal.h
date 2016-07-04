#ifndef __MARSHAL_H
#define __MARSHAL_H


#define LITTLE_ENDIAN_COMNET

#include <CommProto\architecture\os\include_defines.h>

namespace Comnet {
namespace Serialization {

typedef uint8_t* marshall_t;
typedef char* string_t;
typedef wchar_t* wideString_t;

typedef float real32_t;
typedef double real64_t;


uint32_t packString(string_t data, int16_t len, marshall_t input);
void unpackString(string_t data, int16_t len, marshall_t input);

uint32_t packWideString(wideString_t data, int16_t len, marshall_t input);
void unpackWideString(wideString_t data, int16_t len, marshall_t input);

uint32_t packByte(uint8_t data, marshall_t input);
uint8_t unpackByte(marshall_t input);

uint32_t packUint16(uint16_t data, marshall_t input);
uint16_t unpackUint16(marshall_t input);

uint32_t packInt16(int16_t data, marshall_t input);
int16_t unpackInt16(marshall_t input);

uint32_t packUint32(uint32_t data, marshall_t input);
uint32_t unpackUint32(marshall_t input);

uint32_t packInt32(int32_t data, marshall_t input);
int32_t unpackInt32(marshall_t input);

uint32_t packUint64(uint64_t data, marshall_t input);
uint64_t unpackUint64(marshall_t input);

uint32_t packInt64(int64_t data, marshall_t input);
int64_t unpackInt64(marshall_t input);

uint32_t packReal32(real32_t data, marshall_t input);
real32_t unpackReal32(marshall_t input);

uint32_t packReal64(real64_t data, marshall_t input);
real64_t unpackReal64(marshall_t input);


template <typename T>
void swap_endian(T& pX)
{
#ifdef LITTLE_ENDIAN_COMNET
	// should static assert that T is a POD
	char& raw = reinterpret_cast<char&>(pX);
	std::reverse(&raw, &raw + sizeof(T));
#endif
}

template <typename T>
T swap_endian_copy(T pX)
{
	swap_endian(pX);
	return pX;
}


template <typename T>
int16_t packGeneric(T data, marshall_t input)
{
	swap_endian(data);
	memcpy(input, &data, sizeof(T));
	return sizeof(T);	
}

template <typename T>
T unpackGeneric(marshall_t input)
{
	T value;
	memcpy(&value, input, sizeof(T));
	swap_endian(value);
	return value;
}

} // Serialization namespace
} // Comnet namespace
#endif // __MARSHAL_H
