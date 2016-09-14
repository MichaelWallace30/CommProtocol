#include <CommProto/network/UDPLink.h>


namespace Comnet {
namespace Network {



bool UDPLink::initConnection(const char* port, const char* address, uint32_t baudrate) {
  return local.initConnection(port, address);
}


bool UDPLink::addAddress(uint8_t destID, const char* address, uint16_t port) {
  return local.addAddress(destID, address, port);
}


bool UDPLink::removeAddress(uint8_t destID) {
  return local.removeAddress(destID);
}


bool UDPLink::send(uint8_t destID, uint8_t* txData, uint32_t txLength) {
  return local.send(destID, txData, txLength);
}


bool UDPLink::recv(uint8_t* rxData, uint32_t* rxLength) {
  return local.recv(rxData, rxLength);
}
} // namespace Network
} // namespace Comnet