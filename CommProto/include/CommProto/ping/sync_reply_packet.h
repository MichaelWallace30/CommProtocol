/*
Reply to SyncRequestPacket containing the time the request was sent.

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

#ifndef __SYNC_REPLY_PACKET_H
#define __SYNC_REPLY_PACKET_H

#include <CommProto/abstractpacket.h>

namespace comnet {
  namespace ping {
    /**
    Packet to be sent when replying to SyncRequestPacket.
    */
    class SyncReplyPacket : INHERITS_ABSPACKET
    {
    public:
      /**
      Creates a new instance of {@link SyncRequestPacket}.
      @return A new {@link SyncRequestPacket}.
      */
      SyncReplyPacket()
        : CHAIN_ABSPACKET(ABSPacket)
      {

      }

      /**
      requestSentTime is serialized to objOut.
      @param objOut The stream to serialize data to.
      */
      void Pack(REF_OBJECTSTREAM objOut) override;

      /**
      requestSentTime is deserialized from the objIn stream.
      @param objIn The stream to deserialize data from.
      */
      void Unpack(REF_OBJECTSTREAM objIn) override;

      /**
      Modifier for requestSentTime
      */
      void SetRequestSentTime(int32_t requestSentTime)
      {
        this->requestSentTime = requestSentTime;
      }

      /**
      Accessor for requestSentTime
      */
      int32_t GetRequestSentTime()
      {
        return requestSentTime;
      }

      /**
      Creates a new instance of {@link SyncReplyPacket}.
      */
      AbstractPacket* Create() override
      {
        return new SyncReplyPacket();
      }

    private:
      /**
      The timestamp of the peer that sent the SyncRequestPack.
      */
      int32_t requestSentTime;
    };
  }
}

#endif
