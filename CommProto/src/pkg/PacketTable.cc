#include <CommProto/pkg/PacketTable.h>
#include <CommProto/architecture/macros.h>

#if COM_TARGET_OS != COM_OS_WINDOWS
 #include <tr1/functional>
 #define COMMSTD std::tr1
#else 
 #include <functional>
 #define COMMSTD std
#endif


#define DEFAULT_TABLE_SIZE 1024


namespace Comnet {
namespace Pkg {


PacketTable::PacketTable(uint32_t setSize)
: table(new Pair*[setSize])
, numOfPairs(0)
, tableSize(setSize)
{

  nullifyAttributesInTable(table, tableSize);

}


PacketTable::PacketTable() 
: table(new Pair*[DEFAULT_TABLE_SIZE])
, numOfPairs(0)
, tableSize(DEFAULT_TABLE_SIZE)
{

  nullifyAttributesInTable(table, tableSize);

}


PacketTable::~PacketTable()
{
}


bool PacketTable::insert(const AbstractPacket* key, const Callback* callback) {
  if (!key) {
    return false;
  }

  bool stored = false;

  uint32_t hash = (COMMSTD::hash<uint32_t>()(key->getId()) % tableSize);
  
  
  
  return stored;
}


Callback* PacketTable::getCallback(uint32_t key) {
  return NULL;
}


AbstractPacket* PacketTable::getKey(uint32_t key) {
  return NULL;
}


bool PacketTable::remove(uint32_t key) {
  return false;
}


bool PacketTable::resize(uint32_t newSize) {
  return false;
}
} // namespace Pkg
} // namespace Comnet
