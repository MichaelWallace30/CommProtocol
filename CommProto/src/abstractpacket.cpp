#include <CommProto/abstractpacket.h>


// NOTE(): We will need to check this because g++ does not have functional like visual studio.
//#include <tr1/functional>
#include <CommProto/hash/ngcphash.h>
#include <CommProto/architecture/macros.h>

namespace comnet {


/**
   Constructor for AbstractPacket.
*/
AbstractPacket::AbstractPacket(const char* className)
: type_id(hash::ngcp_hash64((const char*)className) % BIT16_VALUE)
  { }


/**
   AbstractPacket Destructor.
*/
AbstractPacket::~AbstractPacket() 
  { }
} // Comnet
