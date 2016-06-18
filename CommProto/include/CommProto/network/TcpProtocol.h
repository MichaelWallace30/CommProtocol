/* 
  TCP configuration.

  Copyright (C) 2016  Michael Wallace, Mario Garcia.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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

  bool initConnection(uint8_t id, PORT port, const char* address = "", uint32_t baudrate = 0);
  
  bool connectToHost(const char* address = "", uint16_t port = 0);
  
  bool disconnect();

  bool sendTo(uint8_t destID, uint8_t* txData, int32_t txLength);

  bool receive(uint8_t* rxData, uint32_t* rxLength);

  bool acceptConnection();

  socket_status_t getSocketStatus() { return tcpSocket.socket_status; }

  struct sockaddr_in& getSocketInput() { return tcpSocket.socket_address; }

  PORT getPortNumber() { return tcpSocket.port; }

  SOCKET getSocketNumber() { return tcpSocket.socket; }

  bool closePort();

  TcpType getType() { return tcpType; }

  void setSocketStatus(socket_status_t status) { tcpSocket.socket_status = status; }

protected:
  
  bool bindSocket();
  bool setNonBlocking(SOCKET socket, bool on);

private:
  socket_t tcpSocket;
  TcpType tcpType;
  List<socket_t>* sockets;
};
} // Network namespace
} // Comnet namespace
#endif // __TCP_PROTOCOL_H
