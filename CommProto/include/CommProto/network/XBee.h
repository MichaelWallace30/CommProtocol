#ifndef __XBEE_H
#define __XBEE_H

#include <CommProto/architecture/connection/xbee_serial.h>

namespace Comnet {
namespace Network {
namespace Experimental {


/**
   XBee communications module. Used for any Xbee protocol connection made to 
   other devices.
*/
class XBee {
public:
  /**
     Default constructor.
   */
  XBee() 
    { }
  /**
     Default destructor.
   */
  ~XBee()
    { }
private:
  /**
   */
  xbee_serial_t serial;
};
} // namespace Experimental
} // namespace Network
} // namespace Comnet
#endif // __XBEE_H
