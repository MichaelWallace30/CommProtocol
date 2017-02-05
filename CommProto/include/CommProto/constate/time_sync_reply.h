/*
Reply to SyncRequestPacket containing the time the request was sent
and more.

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

#ifndef __TIME_SYNC_REPLY_H
#define __TIME_SYNC_REPLY_H

#include <CommProto/constate/connection_state.h>
#include <CommProto/abstractpacket.h>

namespace comnet {
namespace constate {
/**
Packet to reply to {@link TimeSyncRequest}. Contains information vital to
calculate the ping.
*/
class TimeSyncReply : INHERITS_ABSPACKET
{
public:
  /**
  Creates a new instance of {@link TimeSyncReply}.
  */
  TimeSyncReply()
    : CHAIN_ABSPACKET(TimeSyncReply), requestSentTime(0), unixTimeHighResTimeDif(0)
  {
  }

  /**
  Serializes {@link #requestSentTime} and {@link #unixTimeHighResTimeDif}.
  @param objOut The stream to serialize data to.
  */
  void Pack(REF_OBJECTSTREAM objOut) override
  {
    objOut << requestSentTime;
    objOut << unixTimeHighResTimeDif;
  }

  /**
  Deserialized {@link #unixTimeHighResTimeDif} and {@link #requestSentTime}.
  @param objIn The stream to deserialize data from.
  */
  void Unpack(REF_OBJECTSTREAM objIn) override
  {
    objIn >> unixTimeHighResTimeDif;
    objIn >> requestSentTime;
  }

  /**
  Modifier for {@link #requestSentTime}.
  */
  void SetRequestSentTime(int32_t requestSentTime)
  {
    this->requestSentTime = requestSentTime;
  }

  /**
  Set the {@link #unixTimeHighResTimeDif} to the difference between the two arguments.
  @param unixTimeMillis The unix time stamp
  @param highResTimeMillis The time value of the high resolution clock
  */
  void SetTimeDif(UnixMillisInt unixTimeMillis, UnixMillisInt highResTimeMillis)
  {
    this->unixTimeHighResTimeDif = unixTimeMillis - highResTimeMillis;
  }

  /**
  Accessor for {@link #unixTimeHighResTimeDif}.
  */
  UnixMillisInt GetTimeDif()
  {
    return unixTimeHighResTimeDif;
  }

  /**
  Accessor for {@link #requestSentTime}.
  */
  MillisInt GetRequestSentTime()
  {
    return requestSentTime;
  }

  /**
  Creates a new instance of {@link SyncReplyPacket}.
  */
  AbstractPacket* Create() override
  {
    return new TimeSyncReply();
  }

  /**
  Default destructor.
  */
  ~TimeSyncReply()
  {
  }

private:
  /**
  The timestamp of the peer that sent the {@link TimeSyncRequest}.
  */
  MillisInt requestSentTime;

  /**
  The difference between the unix time stamp and the high resolution clock.
  */
  UnixMillisInt unixTimeHighResTimeDif;
};
} //namespace constate
} //namespace comnet

#endif //__TIME_SYNC_REPLY_H
