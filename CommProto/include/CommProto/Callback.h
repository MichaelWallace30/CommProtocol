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
#ifndef __CALLBACK_H
#define __CALLBACK_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/CommsPacket.h>

namespace Comnet {

class AbstractPacket;

// The error number data type.
typedef int error_t;
// Callback function pointer, it is not generic, it is just used as 
// a reference to user defined callback functions.
typedef error_t (*callback_t)(const header_t&, const AbstractPacket&);

/**
   Callback is a class intended to be used for storing functions that request 
   callbacks from within the protocol library.
*/
class Callback {
public:
  /**
     Default Constructor.
   */
  Callback();
  /**
     Call Constructor.
   */
  Callback(callback_t call);

  ~Callback();
  /**
     Set the callback listener in this class.
   */
  void setCallbackListener(callback_t call);
  /**
     Calls the function associated with a packet.
   */
  error_t callFunction(const header_t& header, const AbstractPacket& abPacket);
private:
  callback_t callback;
};
} // namespace Comnet
#endif // __CALLBACK_H
