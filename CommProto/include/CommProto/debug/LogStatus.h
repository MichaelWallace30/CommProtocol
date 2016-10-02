/*
  UDP configurations.

  Copyright (C) 2016  Michael Wallace, Mario Garcia.

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
#ifndef __LOG_STATUS_H
#define __LOG_STATUS_H


namespace comnet {
namespace debug {

class Log;

/**
  The Log status for the Log object.
*/
enum LogStatus {
  LOG_NOTHING,
  LOG_SIMPLE,
  LOG_NOTIFY,
  LOG_WARNING,
  LOG_ERROR,
};


/**
  Identifier for logs.
*/
LogStatus getLogStatus(Log& log);

Log& convertTo(LogStatus status, Log& log);

} // namespace Debug
} // namespace Comnet
#endif //__LOG_STATUS_H