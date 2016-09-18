#include <CommProto/network/XBee.h>

#include <CommProto/architecture/connection/wpan/wpan.h>
#include <CommProto/architecture/macros.h>


namespace Comnet {
namespace Network {


using namespace Experimental;

XBee::XBee()
{
}


bool XBee::initialize(const char* port, speed_t baudrate) {
  bool success;

  if (xbee_open(&serial, baudrate) == XBEE_SUCCESS) {
    success = true;
  }

  return success;  
}


bool XBee::closePort() {
  bool success = false;

  if (xbee_close(&serial) == XBEE_SUCCESS) {
    success = true;
  }

  return success;
}
} // namespace Network
} // namespace Comnet