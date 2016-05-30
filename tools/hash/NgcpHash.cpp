#include <tools/hash/NgcpHash.h>
#include <architecture/macros.h>
#include <architecture/os/arch.h>

/* 
   TODO(): This will need to be defined with windows.
*/ 
#if COM_TARGET_OS == COM_OS_WINDOWS
 #define FORCE_INLINE
#else
 #define FORCE_INLINE static __attribute__((always_inline)) 
#endif // COM_TARGET_OS == COM_OS_WINDOWS

#define NGCP_PRIVATE_API

namespace Comnet {
namespace tools {
namespace hash {

/**
   Reverse the byte binary values.
*/
FORCE_INLINE
ngcp_hash32_t reverse_byte(byte* _byte) {
  ngcp_hash32_t hash = 0;
  hash |= (*_byte & 0x80) >> 7;
  hash |= (*_byte & 0x40) >> 5;
  hash |= (*_byte & 0x20) >> 3;
  hash |= (*_byte & 0x10) >> 1;
  hash |= (*_byte & 0x8) << 1;
  hash |= (*_byte & 0x4) << 3;
  hash |= (*_byte & 0x2) << 5;
  hash |= (*_byte & 0x1) << 7;
  return hash;
}

FORCE_INLINE
bool valueIsNull(void* input) {
  if (input == NULL) {
    return true;
  } else {
    return false;
  }
}

/**
   Hash experimental. Uses researched, custom, non-cryptographic hashing, with a bit of 
   Jenkins avalanche technique. Credit attributed to Jenkins hash. 
*/
ngcp_hash32_t ngcp_hash32(void* input, uint32_t length, unsigned seed) {
  ngcp_hash32_t hash = 0;
  if (valueIsNull(input)) {
    return hash;
  }
  
 unsigned int i = 0;
 byte* p = (byte*)input;
 hash = reverse_byte((p+0));

 while (i < length) {
   hash += *(p+i) << 7 | (hash >> 15);
   hash ^= (hash >> 6);
   i++;
 }
 hash += (hash << 3);
 hash ^= (hash >> 11);
 hash += (hash << 15);
 hash += reverse_byte((p+0));
 return hash;
}

ngcp_hash64_t ngcp_hash64(void* input, uint32_t length, unsigned seed) {
  ngcp_hash64_t hash = 0;
  if (valueIsNull(input)) {
    return hash;
  }

  unsigned int i = 0;
  byte* p = (byte*)input;
  hash = reverse_byte((p+0));
  
  while (i < length) {
    hash += (*(p+i) << 7) | (hash >> 31);
    hash ^= (hash >> 6);
    i++;
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  hash += reverse_byte((p+0));
  return hash;
}
} // hash namespace
} // tools namespace
} // Comnet namespace
