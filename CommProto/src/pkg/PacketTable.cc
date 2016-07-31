#include <CommProto/pkg/PacketHashTable.h>
#include <CommProto/architecture/macros.h>
#include <CommProto/AbstractPacket.h>
#include <CommProto/Callback.h>


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


PacketTable::PacketTable(setSize)
: table(new Pair*[setSize])
, numOfPairs(0)
, tableSize(setSize)
{

  nullifyAttributesInTable(table, tableSize);

}


PacketTable::PacketTable() 
: table(new Pair*[DEFAULT_TABLE_SIZE])
, numOfPairs(0)
, tableSize(setSize)
{

  nullifyAttributesInTable(table, tableSize);

}


bool PacketTable::insert(const AbstractPacket* key, const Callback* callback) {
  if (!key) {
    return false;
  }

  bool stored = false;
  
  
}
} // namespace Pkg
} // namespace Comnet
