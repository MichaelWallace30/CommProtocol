#include <network/XbeeLink.h>

#include <msclr/marshal.h>

namespace Comnet {
namespace Network {


XBeeLink::XBeeLink()
 : node(new comnet::network::XBeeLink())
{
}


XBeeLink::~XBeeLink() 
{
  //free_pointer(node);
}


Boolean XBeeLink::InitConnection(String^ port, String^ address, uint32_t baudrate) {
  msclr::interop::marshal_context ctx;
  const char* portChar = ctx.marshal_as<const char*>(port);
  return node->InitConnection(portChar, "", baudrate);
}


Boolean XBeeLink::AddAddress(uint8_t destId, String^ address, uint16_t port) {
  msclr::interop::marshal_context ctx;
  const char* addressChar = ctx.marshal_as<const char*>(address);
  return node->AddAddress(destId, addressChar, port);
}


Boolean XBeeLink::RemoveAddress(uint8_t destId) {
  return node->RemoveAddress(destId);
}


Boolean XBeeLink::Send(uint8_t destId, uint8_t* txData, uint32_t txLength) {
  return node->Send(destId, txData, txLength);
}


Boolean XBeeLink::Recv(uint8_t* rxData, UInt32% rxLength) {
  uint32_t length = 0;
  bool success = node->Recv(rxData, &length);
  rxLength = length;
  return success;
}
} // Network
} // Comnet