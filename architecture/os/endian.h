#include <architecture/os/include_defines.h>

#if COM_TARGET_OS == COM_OS_WINDOWS

#else   //COM_TRAGET_OS == COM_OS_LINUX || COM_TARGET_OS == COM_OS_APPLE

#include <arpa/inet.h>

#endif

#ifedefine COM_TARGET_OS        COM_OS_WINDOWS
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#else
#endif

#define htonll(x) (
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
(((uint64_t)htonl(x)) << 32) + htonl((x) >> 32)
#endif
)