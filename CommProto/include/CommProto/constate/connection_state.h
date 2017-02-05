/*
Represents a client that needs to be pinged to check if still connected.

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

#ifndef __CONNECTION_STATE_H
#define __CONNECTION_STATE_H

#include <chrono>
#include <stdint.h>
#include <atomic>
#include <CommProto/architecture/os/comm_mutex.h>

namespace comnet {
namespace constate {
/**
Time is represented using high resolution clock so each compiler can
use its most precise clock
*/
typedef std::chrono::high_resolution_clock Time;
//Represents time in milliseconds but must call count to get int form
typedef std::chrono::milliseconds ms;
//Stores a point in time
typedef std::chrono::high_resolution_clock::time_point TimePoint;// std::chrono::time_point <std::chrono::steady_clock> TimePoint;
//Used to store difference between time
typedef std::chrono::duration<float> fsec;
//Integer to store the ping in milliseconds
typedef int16_t PingMillisInt;
//Integer to store general milliseconds
typedef int32_t MillisInt;
//Integer to store unix time stamp in millisecond (yes, it has to be this large)
typedef int64_t UnixMillisInt;

using namespace comnet::architecture::os;

/**When the recalculated (unix time stamp - high res clock)
minus the previous (unix time stamp - high res clock)
has a difference greater than this this amount, resync*/
const MillisInt MAX_UNIX_HIGHRES_TIME_DIF = 2000;
/**The amount of milliseconds to wait before sending another
CheckConnectRequest. Value used when a packet was received from
the peer.*/
const MillisInt CHECK_CONNECT_REQUEST_SEND_DELAY = 7500;
/**The amount of milliseconds to wait before sending another
CheckConnectRequest. Value used when a CheckConnectRequest was
sent to the peer.*/
const MillisInt CHECK_CONNECT_REQUEST_RESEND_DELAY = 2500;
/**The amount of milliseconds that must pass before sending a
CheckConnectReply after sending any packet to the peer*/
const MillisInt CHECK_CONNECT_REPLY_SEND_DELAY = 1500;
/**The number of TimeSyncReplys that must be received for
a ConnectionState to be considered synced*/
const MillisInt NUM_SYNC_REPLIES_TO_SYNC = 8;
/**The amount of time that must pass before sending another
TimeSyncRequest*/
const MillisInt SYNC_REQUEST_SEND_DELAY = 2000;
/**The maximum amount of times a CheckConnectRequest can be
sent without receiving a reply. Once exceeded, the peer is
considered to be inactive*/
const MillisInt MAX_CHECK_CONNECT_REQUESTS = 5;
/**If the ping has this value, then the ping is unknown*/
const PingMillisInt PING_UNKNOWN = -1;

/**Gets a TimePoint representing now*/
TimePoint GetNow();
/**Gets the amount of milliseconds passed between now and
the TimePoint argument*/
MillisInt GetMillisPassed(TimePoint time);
/**Gets the amount of milliseconds passed between now and
the start of the program*/
MillisInt GetTimeSinceStart();
/**Gets the unix time stamp in milliseconds*/
UnixMillisInt GetUnixTimeMillis();

/**The time when the program began*/
const TimePoint START_TIME = GetNow();

/**
Represents the state of a remote comms.
*/
class ConnectionState
{
public:
  /**
  Creates a new instance of {@link ConnectionState}.
  @param nodeID The node id the remote {@link Comms} to represent
  the connection state of.
  */
  ConnectionState(uint8_t nodeID);

  /**
  Accessor for the {@link #nodeID} of the remote {@link Comms} associated 
  with {@code this}.
  @return A copy of {@link #destID}.
  */
  uint8_t GetNodeID() const
  {
    return nodeID;
  }

  /**
  Reset the the time a {@link TimeSyncRequest} was sent to now.
  */
  void ResetSyncRequestSentTime();

  /**
  Gets the amount of milliseconds until a {@link SyncRequestPacket}
  should be sent. Less than or equal to 0 means send a SyncRequestPacket now.
  */
  MillisInt GetTimeUntilSendSyncRequest();

  /**
  Resets the timer to send a {@link CheckConnectRequest} to now, but also sets 
  a delay.  {@link #checkConnectRequestSendDelay} is set to 
  {@link CHECK_CONNECT_SEND_DELAY}. Usually called after
  receiving any packet from the peer.
  */
  void ResetCheckConnectRequestSentTimeToSend();

  /**
  Resets the timer to send a {@link CheckConnectRequest} to now, but also sets
  a delay. {@link #checkConnectRequestSendDelay} is set to 
  {@link CHECK_CONNECT_RESEND_DELAY}. Usually called after
  sending a {@link CheckConnectRequest} to the peer.
  */
  void ResetCheckConnectRequestSentTimeToResend();

  /**
  Gets the amount of time in milliseconds before another {@link CheckConnectRequest} needs to be sent,
  if positive, no packet needs to be send, otherwise send the packet.
  @return The amount of milliseconds before another {@link CheckConnectRequest} should be sent.
  */
  MillisInt GetTimeUntilSendCheckConnectRequest();

  /**
  When active, send {@link CheckConnectRequest}s to ensure a stable connection
  otherwise the connection is considered broken and attempts to get replys will not longer be made.
  @return {@code true} when connection should be checked, {@code false} when the connection is considered broken.
  */
  bool IsActive();

  /**
  The same as {@link #IsActive} except it will not be true until a packet
  has been received from the peer.
  */
  bool IsConnected();

  /**
  Resets the timer to send a {@link CheckConnectReply} to now. Called
  after sending any packet to the peer.
  */
  void ResetCheckConnectReplySentTime();

  /**
  Gets the amount of time in milliseconds before a {@link CheckConnectReply} can be sent,
  If positive, no packet needs to be send, otherwise send the packet.
  @return The amount of milliseconds before another {@link CheckConnectReply} can be sent.
  */
  MillisInt GetTimeUntilSendCheckConnectReply();

  /**
  Compares the time difference passed in as an argument to
  {@link #unixHighResTimeDif}. If their values differ by anything
  more than {@link MAX_UNIX_HIGHRES_TIME_DIF}, return {@code true}.
  {@code true} when resync is necessary, {@code false} otherwise.
  */
  bool IsResyncRequired(UnixMillisInt unixTime);

  /**
  Resets {@link #ping} and {@link #numSyncRepliesReceived} to their
  default state to allow for syncing
  */
  void Resync();

  /**
  Modifier for {@link #unixHighResTimeDif}. Makes sure to call
  after check for resyncing.
  */
  void SetUnixHighResTimeDif(UnixMillisInt unixHighResTimeDif);

  /**
  Sets {@link #timeOff} as an average of itself and the argument.
  @param timeOff The difference between this computers high res time
  and the peers high res time.
  */
  void SyncTime(MillisInt timeOff);

  /**
  True when {@link #numSyncRepliesReceived} is greater than or equal to {@link #NUM_SYNC_REPLIES_TO_SYNC}
  */
  bool IsSynced();

  /**
  True when in {@link TimeSyncManager}s unsynced list, false otherwise.
  */
  bool IsInUnsyncedList();

  /**
  Modifier for {@link #inUnsyncedList}.
  @param mode Value to set {@link #inUnsyncedList} to.
  */
  void SetInUnsyncedList(bool mode);

  /**
  Resets the value of {@link #ping} based on the peers timestamp
  @param time The peer's timestamp
  */
  void UpdatePing(MillisInt time);

  /**
  Accessor for {@link #ping}
  */
  PingMillisInt GetPing();

  /**
  Default destructor.
  */
  ~ConnectionState();

private:
  /**
  The {@link CommNode#node_id} associated with {@code this}. Used to
  check if times should be reset when recieving and sending to a
  {@link CommNode#node_id}.
  */
  const uint8_t nodeID;

  /**
  The last time a {@link TimeSyncRequest} was sent.
  */
  TimePoint lastSyncPackSentTime;

  /**
  The amount of milliseconds that need to pass before sending another
  {@link TimeSyncRequest}.
  */
  MillisInt syncRequestSendDelay;

  /**
  Will be set to current time whenever a packet has been received or a ping has been sent.
  Used by the method {@link #GetTimeUntilSendCheckConnectRequest()}.
  */
  std::atomic<TimePoint> lastCheckConnectRequestSentTime;

  /**
  The amount of milliseconds after {@link #lastCheckConnectRequestSentTime}
  to send another {@link CheckConnectRequest}.
  */
  std::atomic<MillisInt> checkConnectRequestSendDelay;

  /**
  Stores how many times a {@link CheckConnectRequest} has been sent to the {@link #nodeID}
  without receiving any packet.  Reset to 0 by {@link #ResetCheckConnectRequestSentTimeToSend} and
  incremented by {@link #ResetCheckConnectRequestSentTimeToResend}.
  */
  std::atomic<uint8_t> numUnansweredCheckConnectRequests;

  /**
  Stores the last time a packet was sent to the {@link #nodeID}. Used to determine
  whether it is necessary to send a {@link #CheckConnectReply} upon receiving a request.
  */
  std::atomic<TimePoint> lastCheckConnectReplySentTime;

  /**
  True when in {@link TimeSyncManager}s unsynced lists, false otherwise.
  */
  std::atomic<bool> inUnsyncedList;

  /**
  The number of milliseconds the peer's timestamp differs from this computers timestamp.
  */
  MillisInt timeOff;

  /**
  The number of {@link TimeSyncReply} packets received. Determines
  whether the time is synced or not and helps calculate the average
  {@link #timeOff}.
  */
  std::atomic<uint8_t> numSyncRepliesReceived;

  /**
  Stores the last value for the difference between the unix time stamp and the high 
  resolution clock OF THE PEER. This is useful in determining if a resync is required,
  if the peer restarted the program, the high resolution clock may have been reset
  as well, thus changing the value calculated for the ping.
  */
  UnixMillisInt unixHighResTimeDif;

  /**
  The ESTIMATED ping of the peer. Accuracy varies due to thread sleeps in the
  send and receive handlers as well as th ping only being calculated for one
  direction.
  */
  std::atomic<PingMillisInt> ping;

  /**
  True when any packet has ever been received from this peer, false otherwise
  */
  std::atomic<bool> receivedFrom;
};
} //namespace constate
} //namespace comnet
#endif //__PINGER_H
