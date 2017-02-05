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

#ifndef __CHECK_CONNECT_REPLY_H
#define __CHECK_CONNECT_REPLY_H

#include <CommProto/abstractpacket.h>

namespace comnet {
namespace constate {
/**
Packet to reply to CheckConnectRequests, contains no data
*/
class CheckConnectReply : INHERITS_ABSPACKET
{
public:
  /**
  Creates a new instance of {@link CheckConnectReply}.
  */
  CheckConnectReply()
    :CHAIN_ABSPACKET(CheckConnectReply)
  {
  }

  /**
  No data is serialized
  @param objOut The stream to serialize the data to.
  */
  void Pack(REF_OBJECTSTREAM objOut) override
  {
  }

  /**
  No data is deserailized
  @param objIn The stream to parse data from.
  */
  void Unpack(REF_OBJECTSTREAM objIn) override
  {
  }

  /**
  Creates a new instance of {@link CheckConnectReply}.
  @return New instance of {@link CheckConnectReply}.
  */
  AbstractPacket* Create() override
  {
    return new CheckConnectReply();
  }
};
} //namespace constate
} //namespace comnet
#endif //__CHECK_CONNECT_REPLY_H
