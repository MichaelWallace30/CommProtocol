/*
  Console for intercommunications between other programs.

  Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (At your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <CommProto/console/console.h>

#include <CommProto/architecture/macros.h>
#include <CommProto/architecture/os/os_threads.h>

#include <CommProto/network/commsocket.h>

#include <stdio.h>
#include <stdlib.h>

namespace comnet {
namespace console {

Console::Console(const char* address, uint16_t port)
: socket(network::CreateTcpSocket()) 
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
  if (socket->SockSend(command, length) == 0) {
    return true;
  }

  return false;
}


bool Console::listenConnect(uint16_t port) {
  bool success = false;
  if (socket->SockListen(port) == 0) {
    success = true;
  }  

  return success;
}


bool Console::read(char* buffer, int intBuf) {
	//TODO: Removed for socket changes
	/*
  if (socket->SockReceive(buffer, intBuf) == 0) {
    return true;
  }
	*/
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