/*
  Logger used as the control room for operations of CommProtocol.

  Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia.

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
#ifndef __LOGGER_H
#define __LOGGER_H

#include <CommProto/architecture/os/include_defines.h>

namespace Comnet {
namespace Debug {


class Log;
class LoggerManager;


struct log_message_t {
  float timestamp;
  char  message            [64];
  char  messageTitle       [32];
};

/**
  Logger is a message handler for the internals of the CommsLibrary. It is intented to 
  store messages, execute commands when needed, and so on.
*/
class Logger {
public:
  void digestLog(Log* log);
  
  log_message_t obtainMessage(Log* log, log_message_t& message);

  int32_t executeLog(Log* log);

  /**
    Stores the log with an id, which is then returned, as well as stored in this Logger.
   */
  int32_t storeLog(Log* log);

  Log* getLog(int32_t id);

  /**
    Send a command over, without the need to send in a Log object.
   */
  int32_t executeRawCommand(const char* command);
private:

  log_message_t* history [64];
  /**
    Display logs on the command prompt/shell.
   */
  bool notifications;
};
} // namespace Debug
} // namespace Comnet
#endif // __LOGGER_H