/*
  CommProtocol Runtime Logger.  

  Copyright (C) 2016  Mario Garcia.

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
#ifndef __COMMS_LOG_H
#define __COMMS_LOG_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>

#include <string>
#include <vector>

namespace comnet {
namespace debug {


enum LogStatus {
  LOG_DEFAULT   = 0x0,
  LOG_DEBUG     = 0x1,
  LOG_NOTE      = 0x2,
  LOG_NOTIFY    = 0x4,
  LOG_WARNING   = 0x8,
  LOG_ERROR     = 0x10,
  LOG_FATAL     = 0x20,
  LOG_UNKNOWN   = 0x40
};

#define MAX_LOG_PRINTF_BUFFER_SIZE 100

#define LOG_PRINTF(status, msg, ...) \
{ \
	char buffer[MAX_LOG_PRINTF_BUFFER_SIZE];\
	snprintf(buffer, MAX_LOG_PRINTF_BUFFER_SIZE, msg, ##__VA_ARGS__);\
	debug::Log::Message(status, std::string(buffer));\
}
/**
   Log is a notifier. It provides notifications and determines what should or shouldn't be
   displayed.
*/
class COMM_EXPORT Log {
public:
  /**
    Display message to the client.
  */
  static void Message(LogStatus status, const std::string& message);
  /**
    Store message in history.
  */
  static void StoreMessage(LogStatus status, const std::string& message);
  /**
    Dump all messages currently in history, this will display all messages that 
    were previously stored!
  */
	static void Dump();
  /** 
    Clear all of the history!
  */
  static void ClearHistory();
  /**
    Suppress any messages that display the given status. This will prevent 
    any messages from showing to the user.
  */
  static void Suppress(LogStatus status);
  /**
    Unsupress any message status that was previous supressed. This will 
    enable the messages of that given status to display again.
  */
  static void UnSupress(LogStatus status);

private:
  /**
    Suppressed message statuses in the form of a byte. 
  */  
  static char suppressed;
  /**
    History of messages that were stored.
  */
  static std::vector<std::pair<LogStatus, std::string>> history;
};
} // Debug namespace
} // Comnet namespace
#endif // __COMMS_LOG_H
