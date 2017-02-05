/*
Packet to reply to CheckConnectRequests, contains no data

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

#ifndef __CSHARP_CHECK_CONNECT_REPLY_H
#define __CSHARP_CHECK_CONNECT_REPLY_H

#include <vcclr.h>
#using <mscorlib.dll>
#using <System.dll>
#include <ObjectStream.h>
#include <ABSPacket.h>
#include <constate/ConnectionState.h>

namespace Comnet {
namespace Constate {
/**
Packet to reply to CheckConnectRequests, contains no data
*/
public ref class CheckConnectReply : public ABSPacket
{
public:
  /**
  Creates a new instance of {@link CheckConnectReply}.
  */
  CheckConnectReply()
    :ABSPacket("CheckConnectReply")
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
  No data is deserailized
  @param objIn The stream to parse data from.
  */
  Void Unpack(ObjectStream^ objIn) override
  {
  }

  /**
  Creates a new instance of {@link CheckConnectReply}.
  @return New instance of {@link CheckConnectReply}.
  */
  ABSPacket^ Create() override
  {
    return gcnew CheckConnectReply();
  }
};
} //namespace constate
} //namespace comnet
#endif //__CSHARP_CHECK_CONNECT_REPLY_H
