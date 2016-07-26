#include <CommProto/pkg/PackageManager.h>

#define DEFAULT_TABLE_SIZE 1000

namespace Comnet {
namespace Pkg {

PackageManager::PackageManager()
  : table(new Pair*[DEFAULT_TABLE_SIZE])
  , size(0)
{
}

PackageManager::PackageManager(uint32_t setSize)
  : table(new Pair*[setSize])
  , size(0)
{
}

} // namespace Pkg
} // namespace Comnet
