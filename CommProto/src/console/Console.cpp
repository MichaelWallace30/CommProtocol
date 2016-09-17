#include <CommProto/console/Console.h>

#include <CommProto/architecture/macros.h>
#include <CommProto/architecture/os/os_threads.h>

#include <CommProto/network/CommSocket.h>

#include <stdio.h>
#include <stdlib.h>

namespace Comnet {
namespace Console {

Console::Console(const char* address, uint16_t port)
: socket(createTcpSocket()) 
{
  consoles++;
}


Console::~Console() 
{
  free_pointer(socket);
  consoles--;
}


bool Console::createConsole() 
{
  bool result = false;

  return result;
}


bool Console::write(const char* command) {
  int length = 0;
  str_length(command, length);
  if (socket->sockSend(command, length) == 0) {
    return true;
  }

  return false;
}


bool Console::listenConnect(uint16_t port) {
  bool success = false;
  if (socket->sockListen(port) == 0) {
    success = true;
  }  

  return success;
}


bool Console::read(char* buffer, int intBuf) {
  if (socket->sockReceive(buffer, intBuf) == 0) {
    return true;
  }

  return false; 
}


void Console::display() {
  
}


void Console::clr() {
#if defined _WIN32
  system("cls");
#else
  system("clear");
#endif
}


void Console::update() {

}


void Console::start() {
  
}
} // Console namespace
} // Comnet namespace