#include <CommProto/AbstractPacket.h>

// NOTE(): We will need to check this because g++ does not have functional like visual studio.
//#include <tr1/functional>
#include <tools/hash/NgcpHash.h>


namespace Comnet {

/**
   Constructor for AbstractPacket.
*/
AbstractPacket::AbstractPacket(const char* className)
: typeId() {
}

/**
   AbstractPacket Destructor.
*/
AbstractPacket::~AbstractPacket() {
}

} // Comnet
