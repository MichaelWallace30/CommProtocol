/*
  Parsing engine for the Console.

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
#ifndef __PARSE_ENGINE_H
#define __PARSE_ENGINE_H


#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/console/console.h>
#include <string>

namespace comnet {
namespace console {


// Parsing engine for the Console.
class ConsoleParserEngine {
public:
  static ::std::string ParseDecode(Console& console, const char* buffer);

  static bool ParseEncode(Console& console, const char* buffer);
private:
};
} // console
} // comnet 
#endif // __PARSE_ENGINE_H