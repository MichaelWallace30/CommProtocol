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
#include <CommProto/debug/log.h>
#include <CommProto/debug/comms_debug.h>
#include <iostream>
#include <algorithm>

#define PRINT_MESSAGE(status, message) std::cout << #status << ": " << message << std::endl
#define LOG_MESSAGE_CHECK(status, message) \
          case status: \
            if((comnet::debug::Log::suppressed & status) == status) { \
              PRINT_MESSAGE(status, message); \
            } \
          break


namespace comnet {
namespace debug {


std::vector<std::pair<LogStatus, std::string>> Log::history;
char Log::suppressed = 0xe;


void Log::Message(LogStatus status, const std::string& message) {
  switch (status) {
    LOG_MESSAGE_CHECK(LOG_DEFAULT, message);
    LOG_MESSAGE_CHECK(LOG_DEBUG, message);
    LOG_MESSAGE_CHECK(LOG_NOTE, message);
    LOG_MESSAGE_CHECK(LOG_NOTIFY, message);
    LOG_MESSAGE_CHECK(LOG_WARNING, message);
    LOG_MESSAGE_CHECK(LOG_ERROR, message);
    LOG_MESSAGE_CHECK(LOG_FATAL, message);
    default:
      PRINT_MESSAGE(LOG_UNKNOWN, "Unknown message to send");
      break;
  };
}


void Log::ClearHistory() {
  history.clear();
}


void Log::StoreMessage(LogStatus status, const std::string& message) {
  history.push_back(std::make_pair(status, message));
}


void Log::Suppress(LogStatus status) {
  if ((Log::suppressed & status) == status) {
    Log::suppressed ^= status;
  }
}


void Log::UnSupress(LogStatus status) {
  if ((Log::suppressed & status) != status) {
    Log::suppressed |= status;
  }
}


void Log::Dump() {
  std::for_each(history.begin(), history.end(), [] (std::pair<LogStatus, std::string>& msg) -> void {
    Log::Message(msg.first, msg.second);
  });
}
} // debug
} // comnet