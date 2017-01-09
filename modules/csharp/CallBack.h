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

#include <CommProto/callback.h>

#include <ABSPacket.h>
#include <Header.h>

#include <vcclr.h>
#using <mscorlib.dll>


using namespace System;

namespace Comnet {


ref class CommNode;
// Wrapped function pointer.
[UnmanagedFunctionPointerAttribute(CallingConvention::Cdecl)]
public delegate Int32 CommFunct(Header^, ABSPacket^, CommNode^);

// Call back codes.
public enum class CallBackCodes : Int32 {
  CALLBACK_SUCCESS         = CallbackCodes::CALLBACK_SUCCESS,
  CALLBACK_FAIL            = CallbackCodes::CALLBACK_FAIL,
  CALLBACK_DESTROY_PACKET  = CallbackCodes::CALLBACK_DESTROY_PACKET,
};


/**
  Standard callback listener that wraps Callback from within CommProtocol.
*/
public ref class CallBack {
public:
  
  CallBack();
  CallBack(CommFunct^ funct);
  ~CallBack();

  void SetCallBackListenter(CommFunct^ ptr);
  
  Int32 CallFunction(Header^ header, ABSPacket^ packet, CommNode^ node);

private:
  CommFunct^ funct;
};
}
#endif // __CSHARP_CALLACK_H