/*
  Callback Class ADT.

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
#include <CommProto/Callback.h>
#include <CommProto/debug/CommsDebug.h>

#include <stdlib.h>


namespace Comnet {


Callback::Callback(callback_t call)
: callback((callback_t)call)
  { }


Callback::Callback()
: callback(NULL)
  { }


Callback::~Callback() 
  { }


void Callback::setCallbackListener(callback_t call) 
  { callback = call; }


error_t Callback::callFunction(const header_t& header, const AbstractPacket& abPacket) { 
  if (!callback) {
    return -1;
  }
  return callback(header, abPacket); 
}
} // namespace Comnet
