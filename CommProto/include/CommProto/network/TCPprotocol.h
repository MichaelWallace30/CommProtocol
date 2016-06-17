#ifndef __TCP_PROTOCOL_H
#define __TCP_PROTOCOL_H

#include <CommProto/network/CommsLink.h>

#include <CommProto/architecture/connection/socket-config.h>

namespace Comnet {
namespace Network {

class TcpProtocol : public CommsLink {
private:
  socket_t socket;
  
public:
  TcpProtocol();

  ~TcpProtocol();

  bool initConnection(std::string port = "", std::string address = "", uint32_t baudrate = 0);
  
  bool addAddress(uint8_t destID, std::string address = "", uint16_t port = 0);
  
  bool removeAddress(uint8_t destID);

  bool sent(uint8_t destID, uint8_t* txData, int32_t txLength);

  bool recv(uint8_t* rxData, uint32_t* rxLength);

  socket_status_t getSocketStatus() { return socket.socket_status; }

  struct sockaddr_in& getSocketInput() { return socket.socket_address; }

  PORT getPortNumber() { return socket.port; }

  SOCKET getSocketNumber() { return socket.socket; }

  PORT openPort(std::string port);

  bool closePort();
};
} // Network namespace
} // Comnet namespace
#endif // __TCP_PROTOCOL_H
