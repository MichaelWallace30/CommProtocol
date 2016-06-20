#include <CommProto/console/Console.h>

#include <CommProto/architecture/macros.h>
#include <CommProto/architecture/os/os_threads.h>

#include <stdio.h>
#include <stdlib.h>

namespace Comnet {
namespace Console {

Console::Console(const char* address, uint16_t port) {
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
  return false;
}

bool Console::startConnect(const char* address, uint16_t port) {
  return true;
}

bool Console::read(char* buffer, int* intBuf) {
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