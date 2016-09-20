/*
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
#ifndef __CSHARP_CALLBACK_H
#define __CSHARP_CALLBACK_H

#include <CommProto/Callback.h>

#include <ABSPacket.h>
#include <HeaderWrapper.h>

#include <vcclr.h>
#using <mscorlib.dll>


using namespace System;
using namespace System::Runtime::InteropServices;

namespace Comnet {


// Wrapped function pointer.
[UnmanagedFunctionPointerAttribute(CallingConvention::Cdecl)]
public delegate Int32 CommFunct(HeaderWrapper^, ABSPacket^);

// Call back codes.
public enum class CallBackCodes : Int32 {
  CALLBACK_SUCCESS         = 0,
  CALLBACK_FAIL            = 1,
  CALLBACK_STORE_PACKET    = 3,
  CALLBACK_DESTROY_PACKET  = 4,

};


/**
  Standard callback listener that wraps Callback from within CommProtocol.
*/
public ref class CallBack {
public:
  
  CallBack(ABSPacket^ ref);
  CallBack(ABSPacket^ ref, CommFunct^ funct);
  ~CallBack();

  void SetCallBackListenter(CommFunct^ ptr);
  
  Int32 CallFunction(HeaderWrapper^ header, ABSPacket^ packet);
internal:
  const Callback* getUnsafeCallback() {
    return callback;
  }

  error_t helper(const header_t& header, AbstractPacket& packet);
private:
  ABSPacket^ reference;
  CommFunct^ funct;
  Callback* callback;
};
}
#endif // __CSHARP_CALLACK_H