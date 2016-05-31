#include <CommProto/Packets.h>

namespace Comnet {

int CommsPacket::performCalculations(int a) {
  return (id + a);
}
} // Comnet
