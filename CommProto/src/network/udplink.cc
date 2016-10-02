#include <CommProto/network/udplink.h>


namespace comnet {
namespace network {



bool UDPLink::InitConnection(const char* port, const char* address, uint32_t baudrate) {
  return local.InitConnection(port, address);
}


bool UDPLink::AddAddress(uint8_t dest_id, const char* address, uint16_t port) {
  return local.AddAddress(dest_id, address, port);
}


bool UDPLink::RemoveAddress(uint8_t dest_id) {
  return local.RemoveAddress(dest_id);
}


bool UDPLink::Send(uint8_t dest_id, uint8_t* txData, uint32_t txLength) {
  return local.Send(dest_id, txData, txLength);
}


bool UDPLink::Recv(uint8_t* rxData, uint32_t* rxLength) {
  return local.Recv(rxData, rxLength);
}
} // namespace Network
} // namespace Comnet