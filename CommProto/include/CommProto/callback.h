/*
  Callback Class ADT.

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
#ifndef __CALLBACK_H
#define __CALLBACK_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/headerpacket.h>

#include <CommProto/debug/error_status.h>

#include <functional>

namespace comnet {


class AbstractPacket;

// Callback function pointer, it is not generic, it is just used as 
// a reference to user defined callback functions.
typedef error_t (*callback_t)(const Header&, AbstractPacket&);


// Callback codes used for state machines in automating what may be done to the 
// packets.
enum CallbackCodes {
  CALLBACK_SUCCESS                      = 0,
  CALLBACK_FAIL                         = 1,
  CALLBACK_STORE_PACKET                 = 3,
  CALLBACK_DESTROY_PACKET               = 4,
};


/**
   Callback is a class intended to be used for storing functions that request 
   callbacks from within the protocol library.
*/
class Callback {
  typedef std::function < error_t (const Header&, AbstractPacket&) > CallbackFunc;
public:
  /**
     Default Constructor.
   */
  Callback();
  /**
     Call Constructor.
   */
  Callback(CallbackFunc call);

  ~Callback();
  /**
     Set the callback listener in this class.
   */
  void SetCallbackListener(CallbackFunc call);
  /**
     Calls the function associated with a packet.
   */
  error_t CallFunction(const Header& header, const AbstractPacket& abPacket);
private:
  CallbackFunc callback;
};
} // namespace Comnet
#endif // __CALLBACK_H
