#include <CommProto/network/TcpProtocol.h>
#include <iostream>
#include <stdio.h>

// unique code for the message identifier.
#define MESSAGE_CODE 1024

namespace Comnet {
namespace Network {

TcpProtocol::TcpProtocol(TcpType type) 
  : tcpType(type)
{
  tcpSocket.socket = -1;
  tcpSocket.port = -1;
  tcpSocket.socket_status = SOCKET_UNINITIALIZED;
}

TcpProtocol::~TcpProtocol() {
  closePort();
}
#define ADDRESS_LENGTH 64
bool TcpProtocol::initConnection(const char* port, const char* address, uint32_t baudrate) {
  bool result = false;
  initializeSockAPI(result);
  for (int x = 0; port[x] != '\0'; ++x) {
    if (!isdigit(port[x])) {
      printf("initConnection \'port\' argument is not a numerical digit for tcp connection");
      return false;
    }
  }

  uint32_t portInt = atoi(port);

  tcpSocket.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  
  memset((char*)&tcpSocket.socket_address, 0, sizeof(tcpSocket.socket_address));
  tcpSocket.socket_address.sin_family = AF_INET;
  tcpSocket.socket_address.sin_port = htons(portInt);
  tcpSocket.socket_address.sin_addr.s_addr = inet_addr(address);

  if (tcpSocket.socket == -1) {
    printf("socket failed to initialize\n");
    result = false;
  } else {
    if (tcpType == SERVER) {
      if (bind(tcpSocket.socket
          , (struct sockaddr *)&tcpSocket.socket_address
          , sizeof(tcpSocket.socket_address)) == SOCKET_ERROR) 
      {
        printf("Error in binding socket, code: %d\n", GetLastError()); 
        tcpSocket.socket_status = SOCKET_FAILED;
        result = false;
      } else {
        listen(tcpSocket.socket, SOMAXCONN);
        HWND hwnd = FindWindow(NULL, NULL);
        WSAAsyncSelect(tcpSocket.socket, hwnd, MESSAGE_CODE, (FD_ACCEPT 
                                                              | FD_CLOSE 
                                                              | FD_CONNECT 
                                                              | FD_READ 
                                                              | FD_WRITE));
        printf("Listening for connections...\n");

        result = true;
      }
    }
    tcpSocket.socket_status = SOCKET_OPEN;
  }

  return result;
}

bool TcpProtocol::connectToHost(const char* addr, uint16_t port) {
  printf("Client trying to connect to host...\n");
  socket_t target;
  target.socket_address.sin_family = AF_INET;
  target.socket_address.sin_addr.s_addr =  inet_addr(addr);
  target.socket_address.sin_port = htons(port);

  if (connect(tcpSocket.socket, (struct sockaddr*)&target.socket_address, sizeof(target.socket_address)) == SOCKET_ERROR) {
    printf("FAILED CONNECTION\n");
  } else {
    printf("CONNECTION SUCCESS!!\n");
  }
  return true;
}

bool TcpProtocol::removeAddress(uint8_t destID) {
  return false;
}

bool TcpProtocol::sent(uint8_t destID, uint8_t* txData, int32_t txLength) {
  return false;
}

bool TcpProtocol::recv(uint8_t* rxData, uint32_t* rxLength) {
  return false;
}

void TcpProtocol::bindSocket() {
  
}

bool TcpProtocol::closePort() {
  int result = closeSocket(tcpSocket.socket);
  if (result == -1) {
    return false;
  } else {
    return true;
  } 
}
} // Network namespace
} // Comnet namespace
