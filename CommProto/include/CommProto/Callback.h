#ifndef __CALLBACK_H
#define __CALLBACK_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/AbstractPacket.h>
#include <CommProto/CommsPacket.h>

namespace Comnet {


typedef int error_t;
typedef error_t (*callback_t)(const header_t&, const AbstractPacket&);


class Callback {
public:
  Callback();
  Callback(callback_t* call);

  ~Callback();

  void setCallbackListener(callback_t* call);
  /**
     Calls the function associated with a packet.
   */
  error_t callFunction(const header_t& header, const AbstractPacket& abPacket);
private:
  callback_t* callback;
};
} // namespace Comnet
#endif // __CALLBACK_H
