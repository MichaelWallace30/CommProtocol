#include <network/XbeeLink.h>

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
  char* portChar = (char*)(void*)Marshal::StringToHGlobalAnsi(port);
  char* addressChar = (char*)(void*)Marshal::StringToHGlobalAnsi(address);
  return node->InitConnection(portChar, addressChar, baudrate);
}


Boolean XBeeLink::AddAddress(uint8_t destId, String^ address, uint16_t port) {
  char* addressChar = (char*)(void*)Marshal::StringToHGlobalAnsi(address);
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