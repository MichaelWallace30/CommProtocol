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

#include <CommProto/constate/connection_state.h>

namespace comnet {
namespace constate {

TimePoint GetNow() {
  return Time::now();
}

MillisInt GetMillisPassed(TimePoint time) {
  fsec fs = GetNow() - time;	//amount of time that has passed since lastPingTime
  ms millis = std::chrono::duration_cast<ms>(fs);		//converting time to milliseconds
  return (MillisInt)(millis.count());
}

MillisInt GetTimeSinceStart() {
  return GetMillisPassed(START_TIME);
}

UnixMillisInt GetUnixTimeMillis() {
  return std::chrono::duration_cast< std::chrono::milliseconds >(
    std::chrono::system_clock::now().time_since_epoch()).count();
}

ConnectionState::ConnectionState(uint8_t nodeID)
  :nodeID(nodeID), lastSyncPackSentTime(GetNow()),
  syncRequestSendDelay(0), lastCheckConnectRequestSentTime(GetNow()),
  checkConnectRequestSendDelay(0), numUnansweredCheckConnectRequests(0),
  lastCheckConnectReplySentTime(GetNow()), inUnsyncedList(false), 
  timeOff(0), numSyncRepliesReceived(0), ping(PING_UNKNOWN), receivedFrom(false)
{
      
}

void ConnectionState::ResetSyncRequestSentTime()
{
  lastSyncPackSentTime = GetNow();
  syncRequestSendDelay = SYNC_REQUEST_SEND_DELAY;
}

MillisInt ConnectionState::GetTimeUntilSendSyncRequest()
{
  return syncRequestSendDelay - GetMillisPassed(lastSyncPackSentTime);
}

void ConnectionState::ResetCheckConnectRequestSentTimeToSend()
{
  lastCheckConnectRequestSentTime = GetNow();  //Set to current time
  checkConnectRequestSendDelay = CHECK_CONNECT_REQUEST_SEND_DELAY;  //Now that we have received a packet, we no longer have to resend the pingpacket more often so set pingTime back to PING_TIME_MILLIS
  numUnansweredCheckConnectRequests = 0;
  receivedFrom = true;
}

void ConnectionState::ResetCheckConnectRequestSentTimeToResend()
{
  lastCheckConnectRequestSentTime = GetNow();  //Set to current time
  checkConnectRequestSendDelay = CHECK_CONNECT_REQUEST_RESEND_DELAY;  //Now that we have received a packet, we no longer have to resend the pingpacket more often so set pingTime back to PING_TIME_MILLIS
  numUnansweredCheckConnectRequests++;
}

MillisInt ConnectionState::GetTimeUntilSendCheckConnectRequest()
{
  return checkConnectRequestSendDelay - GetMillisPassed(lastCheckConnectRequestSentTime);
}

bool ConnectionState::IsActive()
{
  return numUnansweredCheckConnectRequests <= MAX_CHECK_CONNECT_REQUESTS;
}

bool ConnectionState::IsConnected()
{
  return receivedFrom && IsActive();
}

void ConnectionState::ResetCheckConnectReplySentTime()
{
  lastCheckConnectReplySentTime = GetNow();
}

MillisInt ConnectionState::GetTimeUntilSendCheckConnectReply()
{
  return CHECK_CONNECT_REPLY_SEND_DELAY - GetMillisPassed(lastCheckConnectReplySentTime);
}

bool ConnectionState::IsResyncRequired(UnixMillisInt unixTime)
{
  return numSyncRepliesReceived > 0 && abs(this->unixHighResTimeDif - unixTime) > MAX_UNIX_HIGHRES_TIME_DIF;
}

void ConnectionState::Resync()
{
  ping = PING_UNKNOWN;
  numSyncRepliesReceived = 0;
}

void ConnectionState::SetUnixHighResTimeDif(UnixMillisInt unixHighResTimeDif)
{
  this->unixHighResTimeDif = unixHighResTimeDif;
}

void ConnectionState::SyncTime(MillisInt timeOff)
{
  numSyncRepliesReceived++;
  this->timeOff = (MillisInt)(((float)(numSyncRepliesReceived - 1) / (float)numSyncRepliesReceived) * this->timeOff + 
    (1.0f / (float)numSyncRepliesReceived) * timeOff);
}

bool ConnectionState::IsSynced()
{
  return numSyncRepliesReceived >= NUM_SYNC_REPLIES_TO_SYNC;
}

bool ConnectionState::IsInUnsyncedList()
{
  return inUnsyncedList;
}

void ConnectionState::SetInUnsyncedList(bool mode)
{
  this->inUnsyncedList = mode;
}

void ConnectionState::UpdatePing(MillisInt time)
{
  if (numSyncRepliesReceived > 0)
  {
    ping = GetTimeSinceStart() - (time + timeOff);
    if (ping < 0) {
      ping = 0;
    }
  }
}
PingMillisInt ConnectionState::GetPing()
{
  return ping;
}

ConnectionState::~ConnectionState()
{
}
} //namespace constate
} //namespace comnet
