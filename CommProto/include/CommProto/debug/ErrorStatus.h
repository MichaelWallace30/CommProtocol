/*
  Error status.

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
#ifndef __ERROR_STATUS_H
#define __ERROR_STATUS_H

#include <CommProto/debug/Log.h>

namespace Comnet {
namespace Debug {


/**
  Error codes.
*/
enum ErrorStatus {
  ERR_NONE,
  ERR_NO_REACH,
  ERR_NO_RESPOND,
  ERR_UNKNOWN,
};


/**
  Error Message that can be sent to the Logger.
*/
class Error : public Log {
public:
  
private:
};
} // namespace Debug
} // namespace Comnet
#endif // __ERROR_STATUS_H