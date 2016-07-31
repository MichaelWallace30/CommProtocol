#ifndef __PACKET_FACTORY_H
#define __PACKET_FACTORY_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/macros.h>

#include <CommProto/AbstractPacket.h>

namespace Comnet {
namespace Pkg {


/**
   PacketFactory will be allow to handle the creation of the 
   Packet.
*/
class PacketFactory {
public:  
  PacketFactory() : id(1) { }

  AbstractPacket* produceNewPacket(AbstractPacket* ref) { return NULL; }
 
  
private:
  /**
     PacketFactory id.
   */
  uint32_t id;
};
} // namespace Pkg
} // namespace Comnet
#endif // __PACKET_FACTORY_H
