/*
Packet for requesting that a CheckConnectReply be sent to prove a connection
still exists

Copyright (C) 2016  Alex Craig, Michael Wallace, Mario Garcia.

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

#ifndef __CSHARP_CHECK_CONNECT_REQUEST_H
#define __CSHARP_CHECK_CONNECT_REQUEST_H

#include <vcclr.h>
#using <mscorlib.dll>
#using <System.dll>
#include <ObjectStream.h>
#include <ABSPacket.h>
#include <constate/ConnectionState.h>

namespace Comnet {
namespace Constate {
#pragma managed
using namespace System;
/**
Packet for requesting that a {@link CheckConnectReply} be sent to prove a connection
still exists.
*/
public ref class CheckConnectRequest : public ABSPacket
{
public:
  /**
  Creates a new instance of {@link CheckConnectRequest}.
  */
  CheckConnectRequest()
    :ABSPacket("CheckConnectRequest")
  {
  }

  /**
  No data is serialized
  @param objOut The stream to serialize the data to.
  */
  Void Pack(ObjectStream^ objOut) override
  {
  }

  /**
  No data is deserialized
  @param objIn The stream to parse data from.
  */
  Void Unpack(ObjectStream^ objIn) override
  {
  }

  /**
  Creates a new instance of {@link CheckConnectRequest}.
  @return New instance of {@link CheckConnectRequest}.
  */
  ABSPacket^ Create() override
  {
    return gcnew CheckConnectRequest();
  }
};
} //namespace constate
} //namespace comnet
#endif //__CSHARP_CHECK_CONNECT_REQUEST_H
