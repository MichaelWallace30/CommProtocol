#include <CommProto/pkg/PackageManager.h>
#include <CommProto/AbstractPacket.h>
#include <CommProto/Callback.h>
#include <CommProto/architecture/macros.h>

#if COM_TARGET_OS != COM_OS_WINDOWS
 #include <tr1/functional.h>
 #define COMMSTD std::tr1
#else 
 #include <functional.h>
 #define COMMSTD std
#endif

#define DEFAULT_TABLE_SIZE 1000

namespace Comnet {
namespace Pkg {


PackageManager::PackageManager()
  : table(new Pair*[DEFAULT_TABLE_SIZE])
  , size(0)
  , maxSize(DEFAULT_TABLE_SIZE)
{
  
  nullifyAttributesInTable(table, maxSize);

}


PackageManager::PackageManager(uint32_t setSize)
  : table(new Pair*[setSize])
  , size(0)
  , maxSize(setSize)
{

  nullifyAttributesInTable(table, maxSize);

}


bool PackageManager::insert(const AbstractPacket* key, const Callback* callback) {
  bool success false;
  
  

  return success;
}

Callback* get
} // namespace Pkg
} // namespace Comnet
