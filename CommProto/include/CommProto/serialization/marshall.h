#ifndef __MARSHALL_H
#define __MARSHALL_H

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

marshall_t pack_string(string data, int16 len, marshall_t input);
string unpack_string(marshall_t input, int16 len);

marshall_t pack_byte(uint8 data, marshall_t input);
uint8 unpack_byte(marshall_t input);

marshall_t pack_uint16(uint16 data, marshall_t input);
uint16 unpack_uint16(marshall_t input);

marshall_t pack_int16(int16 data, marshall_t input);
int16 unpack_int16(marshall_t input);

marshall_t pack_uint32(uint32 data, marshall_t input);
uint32 unpack_uint32(marshall_t input);

marshall_t pack_int32(int32 data, marshall_t input);
int32 unpack_int32(marshall_t input);

marshall_t pack_uint64(uint64 data, marshall_t input);
uint64 unpack_uint64(marshall_t input);

marshall_t pack_int64(int64 data, marshall_t input);
int64 unpack_int64(marshall_t input);

mashall_t pack_real32(real32 data, marshall_t input);
real32 unpack_real32(marshall_t input);

mashall_t pack_real64(real64, marshall_t input);
real64 unpack_real64(marshall_t input);
} // Serialization namespace
} // Comnet namespace
#endif // __MARSHALL_H
