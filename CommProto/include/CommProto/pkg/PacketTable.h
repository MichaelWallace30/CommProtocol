#ifndef __PACKET_TABLE_H
#define __PACKET_TABLE_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/macros.h>
#include <CommProto/AbstractPacket.h>
#include <CommProto/Callback.h>


namespace Comnet {
namespace Pkg {


class PacketTable {
public:
  PacketTable(uint32_t setSize);
  PacketTable();

  ~PacketTable();

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

  uint32_t numOfPairs;
  uint32_t tableSize;
};

} // namespace Pkg
} // namespace Comnet

#endif // __PACKET_TABLE_H
