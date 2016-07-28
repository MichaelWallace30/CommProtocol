#ifndef __PACKET_HASH_TABLE_H
#define __PACKET_HASH_TABLE_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/macros.h>

namespace Comnet {
namespace Pkg {


class AbstractPacket;
class Callback;


class PacketHashTable {
public:
  PacketHashTable(uint32_t setSize);

  bool insert(const AbstractPacket* key, const Callback* callback);

  Callback* getCallback(uint32_t key);

  AbstractPacket* getKey(uint32_t key);

  bool remove(uint32_t key);

  bool resize(uint32_t newSize);
private:

  struct Pair {
    Callback* callback;
    AbstractPacket* packet;
  };

  Pair** table;
};

} // namespace Pkg
} // namespace Comnet

#endif // __PACKET_HASH_TABLE_H
