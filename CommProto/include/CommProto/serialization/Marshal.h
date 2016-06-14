#ifndef __MARSHAL_H
#define __MARSHAL_H

namespace Comnet {
namespace Serialization {

typedef unsigned char* marshall_t;
typedef char* string_t;

typedef unsigned char uint8;
typedef char int8;

typedef unsigned short uint16;
typedef short int16;

typedef unsigned int uint32;
typedef int int32;

typedef unsigned long long uint64;
typedef long long int64;

typedef float real32;
typedef double real64;

// Marshalling values.

marshall_t packString(string_t data, int16 len, marshall_t input);
string_t unpackString(marshall_t input, int16 len);

marshall_t packByte(uint8 data, marshall_t input);
uint8 unpackByte(marshall_t input);

marshall_t packUint16(uint16 data, marshall_t input);
uint16 unpackUint16(marshall_t input);

marshall_t packInt16(int16 data, marshall_t input);
int16 unpackInt16(marshall_t input);

marshall_t packUint32(uint32 data, marshall_t input);
uint32 unpackUint32(marshall_t input);

marshall_t packInt32(int32 data, marshall_t input);
int32 unpackInt32(marshall_t input);

marshall_t packUint64(uint64 data, marshall_t input);
uint64 unpackUint64(marshall_t input);

marshall_t packInt64(int64 data, marshall_t input);
int64 unpackInt64(marshall_t input);

marshall_t packReal32(real32 data, marshall_t input);
real32 unpackReal32(marshall_t input);

marshall_t packReal64(real64, marshall_t input);
real64 unpackReal64(marshall_t input);
} // Serialization namespace
} // Comnet namespace
#endif // __MARSHAL_H
