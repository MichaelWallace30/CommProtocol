#include <CommProto/network/Tcp.h>

namespace Comnet {
namespace Network {

TcpProtocol::TcpProtocol() 
{
  socket.socket = -1;
  socket.port = -1;
  socket.socket_status = SOCKET_UNINITIALIZED;
}

bool TcpProtocol::initConnection(std::string port, std::string address, uint32_t baudrate) {
}


} // Network namespace
} // Comnet namespace
