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

#pragma once
#include <CommProto/headerpacket.h>
#include <vcclr.h>
#using <mscorlib.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace Comnet{
#pragma managed	
public ref class Header sealed
{
 public:
  comnet::Header *header;	
  Header();
    Header(comnet::Header* header);
  ~Header();
  void SetDestID(Byte destID);
  uint8_t GetDestID();
  void SetSourceID(Byte sourceID);
  uint8_t GetSourceID();
  void SetMessageLength(UInt16 mesageLength);
  uint16_t GetMessageLength();
  void SetMessageID(UInt16 messageID);
  uint16_t GetMessageID();
  int32_t GetSourceTime();
  void SetIV(cli::array<Byte>^ IV);		
  cli::array<Byte>^ GetIV();

    comnet::Header* GetHeader() {
      return header;
    }
 };
}
