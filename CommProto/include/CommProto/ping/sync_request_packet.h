/*
Packet for syncing time.

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

#ifndef __SYNC_REQUEST_PACKET_H
#define __SYNC_REQUEST_PACKET_H

#include <CommProto/abstractpacket.h>
#include <string>

namespace comnet {
  namespace ping {
    /**
    Packet to be sent when requesting the timestamp of the peer.
    */
    class SyncRequestPacket : INHERITS_ABSPACKET
    {
    public:
      /**
      Creates a new instance of {@link SyncRequestPacket}.
      @return A new {@link SyncRequestPacket}.
      */
      SyncRequestPacket();

      /**
      No data is serialized.
      @param objOut The stream to serialize the data to.
      */
      void Pack(REF_OBJECTSTREAM objOut) override;

      /**
      No data is parsed.
      @param objIn The stream to parse data from.
      */
      void Unpack(REF_OBJECTSTREAM objIn) override;

						void SetTimeDif(int64_t unixTimeMillis, int64_t highResTimeMillis)
						{
								this->unixTimeHighResTimeDif = unixTimeMillis - highResTimeMillis;
						}

						int64_t GetTimeDif() {
								return this->unixTimeHighResTimeDif;
						}

      /**
      Creates a new instance of {@link SyncRequestPacket}.
      @return New instance of {@link SyncRequestPacket}.
      */
						AbstractPacket* Create() override
						{
								return new SyncRequestPacket();
						}

				private:
						int64_t unixTimeHighResTimeDif;
    };
  } //namespace ping
} //namespace comnet
#endif //__PING_PACKET_H
