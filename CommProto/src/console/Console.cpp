#include <CommProto/console/Console.h>

#include <CommProto/architecture/macros.h>
#include <CommProto/architecture/os/os_threads.h>

#include <stdio.h>
#include <stdlib.h>

namespace Comnet {
namespace Console {

Console::Console(const char* address, PORT port) {
  tcp.initConnection(consoles, port, address, 0);
  consoles++;
}

Console::~Console() {
}

bool Console::createConsole() 
{
  bool result = false;

  return result;
}

bool Console::write(const char* command) {
  int length = 0;
  str_length(command, length);
  return tcp.sendTo(1, (uint8_t*)command, length);
}

bool Console::startConnect(const char* address, PORT port) {
  return tcp.connectToHost(address, port);
}

bool Console::read(char* buffer, int* intBuf) {
  return tcp.receive((uint8_t*)buffer, (uint32_t*)intBuf); 
}

void Console::display() {
  
}

void Console::clr() {
}

void Console::update() {
}

void Console::start() {
  
}
} // Console namespace
} // Comnet namespace