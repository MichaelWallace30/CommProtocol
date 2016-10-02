#include <network/CXbeeLink.h>

namespace Comnet {
namespace Network {


CXBeeLink::CXBeeLink()
: node(new experimental::XBeeLink())
{
}


CXBeeLink::~CXBeeLink() 
{
  free_pointer(node);
}


Boolean CXBeeLink::InitConnection(String^ port, String^ address, uint32_t baudrate) {
  char* portChar = (char*)(void*)Marshal::StringToHGlobalAnsi(port);
  char* addressChar = (char*)(void*)Marshal::StringToHGlobalAnsi(address);
  return node->initConnection(portChar, addressChar, baudrate);
}


Boolean CXBeeLink::AddAddress(uint8_t destId, String^ address, uint16_t port) {
  char* addressChar = (char*)(void*)Marshal::StringToHGlobalAnsi(address);
  return node->addAddress(destId, addressChar, port);
}


Boolean CXBeeLink::RemoveAddress(uint8_t destId) {
  return node->removeAddress(destId);
}


Boolean CXBeeLink::Send(uint8_t destId, uint8_t* txData, uint32_t txLength) {
  return node->send(destId, txData, txLength);
}


Boolean CXBeeLink::Recv(uint8_t* rxData, UInt32% rxLength) {
  uint32_t length = 0;
  bool success = node->recv(rxData, &length);//probably wont work
  rxLength = length;
  return success;
}
} // Network
} // Comnet