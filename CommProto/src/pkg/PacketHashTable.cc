#include <CommProto/pkg/PacketHashTable.h>
#include <CommProto/architecture/macros.h>
#include <CommProto/AbstractPacket.h>
#include <CommProto/Callback.h>


#if COM_TARGET_OS != COM_OS_WINDOWS
 #include <tr1/functional.h>
 #define COMMSTD std::tr1
#else 
 #include <functional.h>
 #define COMMSTD std
#endif


namespace Comnet {
namespace Pkg {



} // namespace Pkg
} // namespace Comnet
