#include <CommProto/pkg/PacketManager.h>
#include <CommProto/pkg/PacketTable.h>
#include <CommProto/AbstractPacket.h>
#include <CommProto/Callback.h>

#define DEFAULT_TABLE_SIZE 1000

namespace Comnet {
namespace Pkg {


PacketManager::PacketManager()
  : table(new PacketTable(DEFAULT_TABLE_SIZE))
{
}


PacketManager::PacketManager(uint32_t setSize)
  : table(new PacketTable(setSize))
{
}


PacketManager::~PacketManager() 
{
  delete table;
  nullify_pointer(table);
}


bool PacketManager::insert(const AbstractPacket* key, const Callback* callback) {
  return table->insert(key, callback);
}


Callback* PacketManager::get(const AbstractPacket& key) {
  return table->getCallback(key.getId());
}


AbstractPacket* PacketManager::produceFromId(uint32_t key) {
  AbstractPacket* packet = table->getPacket(key);
  return factory.produceNewPacket(packet);
}


bool PacketManager::remove(const AbstractPacket& key) {
  return table->remove(key.getId());
}
} // namespace Pkg
} // namespace Comnet
