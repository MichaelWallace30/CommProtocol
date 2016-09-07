#include <CommProto/serialization/Marshal.h>//endianess

#ifndef CRC32
#define CRC32

namespace Comnet {
namespace Network {
/** CRC32 checksum function*/
	unsigned int crc32(unsigned char *message, int length);
	void appendCrc32(uint8_t* buffer, uint32_t *length);
	unsigned int truncateCrc32(uint8_t* buffer, uint32_t *length);
}//end NETWORK namespace
}//end COMNET namespace
#endif
