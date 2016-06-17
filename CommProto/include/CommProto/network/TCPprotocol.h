#ifndef __TCP_PROTOCOL_H
#define __TCP_PROTOCOL_H

#include <CommProto/network/CommsLink.h>

#include <CommProto/architecture/connection/socket-config.h>

namespace Comnet {
namespace Network {

enum TcpType {
  CLIENT,
  SERVER = 0601,
};

class TcpProtocol : public CommsLink {  
public:
  TcpProtocol(TcpType type = CLIENT);

  ~TcpProtocol();

  bool initConnection(std::string port = "", std::string address = "", uint32_t baudrate = 0);
  
  bool addAddress(uint8_t destID, std::string address = "", uint16_t port = 0);
  
  bool removeAddress(uint8_t destID);

  bool sent(uint8_t destID, uint8_t* txData, int32_t txLength);

  bool recv(uint8_t* rxData, uint32_t* rxLength);

  socket_status_t getSocketStatus() { return tcpSocket.socket_status; }

  struct sockaddr_in& getSocketInput() { return tcpSocket.socket_address; }

  PORT getPortNumber() { return tcpSocket.port; }

  SOCKET getSocketNumber() { return tcpSocket.socket; }

  bool closePort();

  TcpType getType() { return tcpType; }

protected:
  
  void bindSocket();

private:
  socket_t tcpSocket;
  TcpType tcpType;
};
} // Network namespace
} // Comnet namespace
#endif // __TCP_PROTOCOL_H
