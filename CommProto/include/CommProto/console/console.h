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
#ifndef __CONSOLE_H
#define __CONSOLE_H

#define MAX_PATH 260
#define DEFAULT_HELPER_EXE "cmd"

#include <CommProto/network/commsocket.h>

namespace comnet {
namespace console {

using namespace comnet::network;
/**
   Console commands for use with the debug, administration console.
*/
enum ConsoleCommands {
  CONSOLE_OPEN         = 0000,
  CONSOLE_CLOSE        = 0001,
  CONSOLE_CLEAR        = 0002,
  CONSOLE_CLEAR_SCREEN = 0003,
  CONSOLE_HOP          = 0004,
  CONSOLE_PRINT        = 0005,
  CONSOLE_TIME         = 0006,
  CONSOLE_PING         = 0007,
  CONSOLE_RESET        = 0010,
  CONSOLE_INIT         = 0011,
  CONSOLE_GOTO         = 0012,
  CONSOLE_IMPORT       = 0013,
  CONSOLE_EXPORT       = 0014,
};

/**
  Keep track on the number of consoles currently open.
*/
static int consoles = 0;

/**
   Basic console interface. Intended to call a console device. 
   this will allow user to administrate connection handling, import sheets during runtime (if possible), as 
   well as perform numerous, safe options to will offer better performance to the user's specifications.

  This console object does not Create a separate process, instead it creates a thread that will allow the user
  to enter as a means to check the communication signal and quality, as well as perform certain commands.

  In order to Create separate processes for use of this console namely for remote access to this library, 
  be sure to call this class within another project folder, and call the process from the hosting program.

  calling processes is done by function within a program, for windows it is CreateProcess, for linux it is fork().
*/
class COMM_EXPORT Console {
public:
  Console(const char* address, uint16_t port);
  virtual ~Console();

  /**
     Create the console via TCP connection.
   */
  virtual bool createConsole();
  /**
     Write into the console. This will allow the user to Send commands to the program, by means of a buffer.
   */
  virtual bool write(const char* command = "");

  /**
     Reads output from the program, Back to the user for evaluation.
   */
  virtual bool read(char* buffer, int intBuf);

  /**
    Start the connection via tcp.
   */
  virtual bool listenConnect(uint16_t port);
  
  /**
     Display, which will display information Back to the user. Acts as a refresher to update the console.
   */
  virtual void display();
  /**
     Clear the screen.
   */
  virtual void clr();

  /** 
    Starts the console thread.
   */
  virtual void start();
protected:
  /**
     Update the console window.
   */
  virtual void update();

protected:
  /**
    The name of this console.
   */
  char* console_title;
  /**
    Socket used to define this Console object.
   */
  CommSocket* socket;
  /**
    The unique id of this Console object.
   */
  uint32_t id;
};
}
}

#endif // __CONSOLE_H
