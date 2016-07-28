#ifndef __PACKET_FACTORY_H
#define __PACKET_FACTORY_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/macros.h>

namespace Commet {
namespace Pkg {


class AbstractPacket;

/**
   PacketFactory will be allow to handle the creation of the 
   Packet.
*/
class PacketFactory {
public:  
  PacketFactory() { }

  AbstractPacket* produceNewPacket(AbstractPacket* ref);
 
  
private:
  /**
     PacketFactory id.
   */
  uint32_t id;
};
} // namespace Pkg
} // namespace Comnet
#endif // __PACKET_FACTORY_H
