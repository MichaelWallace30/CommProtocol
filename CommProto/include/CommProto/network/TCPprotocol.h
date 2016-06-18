#ifndef __TCP_PROTOCOL_H
#define __TCP_PROTOCOL_H

#include <CommProto/architecture/connection/socket-config.h>

#include <CommProto/tools/data_structures/interface/InterfaceList.h>

namespace Comnet {
namespace Network {

using namespace Comnet::Tools::DataStructures::Interface;

enum TcpType {
  CLIENT,
  SERVER = 0601,
};

/**
  TCP connection class, intended for single connections purposes, however, setting 
  the tcpType as SERVER will allow the class to accept multiple connections. 
*/
class TcpProtocol {  
public:
  TcpProtocol(TcpType type = CLIENT);

  ~TcpProtocol();

  bool initConnection(uint8_t id, const char* port = "", const char* address = "", uint32_t baudrate = 0);
  
  bool connectToHost(const char* address = "", uint16_t port = 0);
  
  bool disconnect(uint16_t port, const char* address);

  bool sendTo(uint8_t destID, uint8_t* txData, int32_t txLength);

  bool receive(uint8_t* rxData, uint32_t* rxLength);

  bool acceptConnection();

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
  SOCKET sock;
  List<socket_t>* sockets;
};
} // Network namespace
} // Comnet namespace
#endif // __TCP_PROTOCOL_H
