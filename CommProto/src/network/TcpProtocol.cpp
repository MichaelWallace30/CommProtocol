#include <CommProto/network/TcpProtocol.h>
#include <CommProto/tools/data_structures/DoubleLinkedList.h>
#include <CommProto/console/CommsDebug.h>

#include <iostream>
#include <stdio.h>

// unique code for the message identifier.
#define MESSAGE_CODE 2056

namespace Comnet {
namespace Network {

/**
  Socket Comparator, to compare sockets with eachother in the data structure.
 */  
class SocketComparator : public Comnet::Tools::Comparator<socket_t> {
public:
  SocketComparator() { }

  int32_t compare(const socket_t& s1, const socket_t& s2) {
    if ((s1.socket_address.sin_addr.s_addr == s2.socket_address.sin_addr.s_addr) &&
        (s1.port == s2.port) && (s1.id == s2.id))
    {
      return 1;
    } else {
      return 0;
    }
  }

  bool equal(const socket_t& s1, const socket_t& s2) {
    return compare(s1, s2);
  }
};


using namespace Comnet::Tools::DataStructures;

TcpProtocol::TcpProtocol(TcpType type) 
  : tcpType(type)
  , sockets(new DoubleLinkedList<socket_t, SocketComparator>())
{
  tcpSocket.socket = -1;
  tcpSocket.port = -1;
  tcpSocket.socket_status = SOCKET_UNINITIALIZED;
}

TcpProtocol::~TcpProtocol() {
  closePort();  
  delete sockets;
  sockets = NULL;
}

bool TcpProtocol::initConnection(uint8_t id, const char* port, const char* address, uint32_t baudrate) {
  bool result = false;
  initializeSockAPI(result);
  for (int x = 0; port[x] != '\0'; ++x) {
    if (!isdigit(port[x])) {
      comms_debug_log("initConnection \'port\' argument is not a numerical digit for tcp connection");
      return false;
    }
  }

  uint32_t portInt = atoi(port);

  tcpSocket.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  
  memset((char*)&tcpSocket.socket_address, 0, sizeof(tcpSocket.socket_address));
  tcpSocket.socket_address.sin_family = AF_INET;
  tcpSocket.socket_address.sin_port = htons(portInt);
  tcpSocket.socket_address.sin_addr.s_addr = inet_addr(address);
  tcpSocket.id = id;

  if (tcpSocket.socket == -1) {
    comms_debug_log("socket failed to initialize\n");
    result = false;
  } else {
    if (tcpType == SERVER) {
      result = bindSocket();
    } else {
      result = true;
      tcpSocket.socket_status = SOCKET_OPEN;
    }
  }

  return result;
}

bool TcpProtocol::acceptConnection() {
  bool result = false;
  socket_t temp;
  socklen_t size = sizeof(temp.socket_address);
  SOCKET sock = accept(tcpSocket.socket, (struct sockaddr*)&temp.socket_address , &size);
  temp.socket = sock;

  if (sock == INVALID_SOCKET) {
    comms_debug_log("Socket failed\n"); 
    closePort();
  } else {
    comms_debug_log("Socket succeeded!\n");
    sockets->insert(temp);
  }
  
  return result;
}

bool TcpProtocol::connectToHost(const char* addr, uint16_t port) {
  comms_debug_log("Client trying to connect to host...\n");
  socket_t target;
  target.socket_address.sin_family = AF_INET;
  target.socket_address.sin_addr.s_addr =  inet_addr(addr);
  target.socket_address.sin_port = htons(port);
  target.port = port;

  if (connect(tcpSocket.socket
              , (struct sockaddr*)&target.socket_address
              , sizeof(target.socket_address)) == -1) 
  {
    comms_debug_log("FAILED CONNECTION\n");
    closePort();
  } else {
    comms_debug_log("CONNECTION SUCCESS!!\n");
    
    sockets->insert(target);
  }
  return true;
}

bool TcpProtocol::disconnect(uint16_t port, const char* address) {
  return false;
}

bool TcpProtocol::sendTo(uint8_t destID, uint8_t* txData, int32_t txLength) {
  bool result = false;
  SOCKET target = INVALID_SOCKET;
  for (int i = 0; i < sockets->getSize(); ++i) {
    socket_t& temp = sockets->at(i);
    if (temp.id == destID) {
      target = temp.socket;
      break;
    }
  } 
  
  if (send(tcpSocket.socket, (const char*)txData, txLength, 0) != -1) {
    comms_debug_log("Message Sent!\n");
    result = true;
  } else {
    comms_debug_log("Message not sent!\n");
  }
  return result;
}

bool TcpProtocol::receive(uint8_t* rxData, uint32_t* rxLength) {
  bool result = false;
  for (int i = 0; i < sockets->getSize(); ++i) {
    socket_t& temp = sockets->at(i);
    if (recv(temp.socket, (char*)rxData, *rxLength, 0) != -1) {
      comms_debug_log("Successfull Retrieval\n");
      break;
    }
  } 

  return result;
}

bool TcpProtocol::bindSocket() {
  bool result = false;
  if(bind(tcpSocket.socket
     , (struct sockaddr *)&tcpSocket.socket_address
     , sizeof(tcpSocket.socket_address)) == SOCKET_ERROR)
  {
    comms_debug_log("Error in binding socket\n");
    tcpSocket.socket_status = SOCKET_FAILED;
  } else {
    listen(tcpSocket.socket, SOMAXCONN);

    comms_debug_log("Listening for connections...\n");

    result = true;
  }
  return result;
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
