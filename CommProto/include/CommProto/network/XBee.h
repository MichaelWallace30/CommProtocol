#ifndef __XBEE_H
#define __XBEE_H

#include <CommProto/network/CommsLink.h>

namespace Comnet {
namespace Network {


/**
   XBee communications module. Used for any Xbee protocol connection made to 
   other devices.
*/
class Xbee : public CommsLink {
public:
  /**
     Default constructor.
   */
  XBee();
  /**
     Default destructor.
   */
  ~XBee();
  /**
     Initialize a connection for this node.
   */
  bool initConnection(const char* port = NULL, const char* = NULL, uint32_t baudrate = 0);
  /**
     Adds an address to the link.
   */
  bool addAddress(uint8_t destId, const char* address = NULL, int16_t port = 0);
  /**
     Remove an address from the link.
   */
  bool removeAddress(uint8_t destId);
  /**
     Send a message to the specified id of which an address is associated with.
   */
  bool send(uint8_t destId, uint8_t* txData, int32_t txLength);
  /**
     Receive a message from a protocol id.
   */
  bool recv(uint8_t* rxData, uint32_t* rxLength);
private:

};
} // namespace Network
} // namespace Comnet
#endif // __XBEE_H
