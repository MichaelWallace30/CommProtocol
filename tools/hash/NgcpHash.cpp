#include <tools/hash/NgcpHash.h>
#include <architecture/macros.h>
#include <architecture/os/arch.h>

/* 
   TODO(): This will need to be defined with windows.
*/ 
#if COM_TARGET_OS == COM_OS_WINDOWS
 #define FORCE_INLINE
#else
 #define FORCE_INLINE static __attributes__((__always_inline)) 
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


} // hash
} // tools
} // Comnet
