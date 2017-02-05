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

#ifndef __TIME_SYNC_REQUEST_H
#define __TIME_SYNC_REQUEST_H

#include <CommProto/abstractpacket.h>
#include <CommProto/constate/connection_state.h>

namespace comnet {
namespace constate {
/**
Packet to be sent when requesting the timestamp of the peer.
*/
class TimeSyncRequest : INHERITS_ABSPACKET
{
public:
  /**
  Creates a new instance of {@link SyncRequestPacket}.
  @return A new {@link SyncRequestPacket}.
  */
  TimeSyncRequest()
    :CHAIN_ABSPACKET(TimeSyncRequest), unixTimeHighResTimeDif(0)
  {
  }

  /**
  Serialize the {@link #unixTimeHighResTimeDif}.
  @param objOut The stream to serialize the data to.
  */
  void Pack(REF_OBJECTSTREAM objOut) override
  {
    objOut << unixTimeHighResTimeDif;
  }

  /**
  Deserialize the {@link #unixTimeHighResTimeDif}.
  @param objIn The stream to parse data from.
  */
  void Unpack(REF_OBJECTSTREAM objIn) override
  {
    objIn >> unixTimeHighResTimeDif;
  }

  /**
  Set the {@link #unixTimeHighResTimeDif} to the difference between the two arguments.
  @param unixTimeMillis The unix time stamp
  @param highResTimeMillis The time value of the high resolution clock
  */
  void SetTimeDif(UnixMillisInt unixTimeMillis, UnixMillisInt highResTimeMillis) {
    this->unixTimeHighResTimeDif = unixTimeMillis - highResTimeMillis;
  }

  /**
  Accessor for {@link #unixTimeHighResTimeDif}.
  */
  UnixMillisInt GetTimeDif() {
    return this->unixTimeHighResTimeDif;
  }

  /**
  Creates a new instance of {@link TimeSyncRequest}.
  @return New instance of {@link TimeSyncRequest}.
  */
  AbstractPacket* Create() override {
    return new TimeSyncRequest();
  }

  /**
  Default destructor.
  */
  ~TimeSyncRequest()
  {
  }

private:
  /**
  The difference between the unix time stamp and the high resolution clock.
  */
    UnixMillisInt unixTimeHighResTimeDif;
};
} //namespace constate
} //namespace comnet
#endif //__TIME_SYNC_REQUEST_H
