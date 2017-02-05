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

#include "ConnectionState.h"
#include <math.h>

namespace Comnet {
namespace Constate {
MillisInt GetMillisPassed(Diagnostics::Stopwatch ^ timer) {
  return (MillisInt)timer->ElapsedMilliseconds;
}
MillisInt GetTimeSinceStart() {
  return (MillisInt)ConnectionState::START_TIME->ElapsedMilliseconds;
}

UnixMillisInt GetUnixTimeMillis() {
  return (DateTime::UtcNow - DateTime(1970, 1, 1)).TotalMilliseconds;
}

ConnectionState::ConnectionState(uint8_t nodeID)
  :nodeID(nodeID), syncRequestSendDelay(0), checkConnectRequestSendDelay(0),
  numUnansweredCheckConnectRequests(0), inUnsyncedList(false), timeOff(0),
  numSyncRepliesReceived(0), ping(PING_UNKNOWN), receivedFrom(false)
{
  lastSyncPackSentTimer = Diagnostics::Stopwatch::StartNew();
  lastCheckConnectRequestSentTimer = Diagnostics::Stopwatch::StartNew();
  checkConnectRequestMutex = gcnew Threading::Mutex();
  lastCheckConnectReplySentTimer = Diagnostics::Stopwatch::StartNew();
  checkConnectReplyMutex = gcnew Threading::Mutex();
  pingMutex = gcnew Threading::Mutex();
}

void ConnectionState::ResetSyncRequestSentTime()
{
  lastSyncPackSentTimer->Restart();
  syncRequestSendDelay = SYNC_REQUEST_SEND_DELAY;
}

MillisInt ConnectionState::GetTimeUntilSendSyncRequest()
{
  return syncRequestSendDelay - GetMillisPassed(lastSyncPackSentTimer);
}

void ConnectionState::ResetCheckConnectRequestSentTimeToSend()
{
  checkConnectRequestMutex->WaitOne();
  lastCheckConnectRequestSentTimer->Restart();  //Set to current time
  checkConnectRequestSendDelay = CHECK_CONNECT_REQUEST_SEND_DELAY;  //Now that we have received a packet, we no longer have to resend the pingpacket more often so set pingTime back to PING_TIME_MILLIS
  numUnansweredCheckConnectRequests = 0;
  receivedFrom = true;
  checkConnectRequestMutex->ReleaseMutex();
}

void ConnectionState::ResetCheckConnectRequestSentTimeToResend()
{
  checkConnectRequestMutex->WaitOne();
  lastCheckConnectRequestSentTimer->Restart();  //Set to current time
      
  checkConnectRequestSendDelay = CHECK_CONNECT_REQUEST_RESEND_DELAY;  //Now that we have received a packet, we no longer have to resend the pingpacket more often so set pingTime back to PING_TIME_MILLIS
  numUnansweredCheckConnectRequests++;
  checkConnectRequestMutex->ReleaseMutex();
}

MillisInt ConnectionState::GetTimeUntilSendCheckConnectRequest()
{
  checkConnectRequestMutex->WaitOne();
  MillisInt timeUntil = checkConnectRequestSendDelay - GetMillisPassed(lastCheckConnectRequestSentTimer);
  checkConnectRequestMutex->ReleaseMutex();
  return timeUntil;
}

bool ConnectionState::IsActive()
{
  checkConnectRequestMutex->WaitOne();
  bool active = numUnansweredCheckConnectRequests <= MAX_CHECK_CONNECT_REQUESTS;
  checkConnectRequestMutex->ReleaseMutex();
  return active;
}

bool ConnectionState::IsConnected()
{
  checkConnectRequestMutex->WaitOne();
  bool receivedFromCpy = receivedFrom;
  checkConnectRequestMutex->ReleaseMutex();
  return receivedFromCpy && IsActive();
}

void ConnectionState::ResetCheckConnectReplySentTime()
{
  checkConnectReplyMutex->WaitOne();
  lastCheckConnectReplySentTimer->Restart();
  checkConnectReplyMutex->ReleaseMutex();
}

MillisInt ConnectionState::GetTimeUntilSendCheckConnectReply()
{
  checkConnectReplyMutex->WaitOne();
  MillisInt timeUntil = CHECK_CONNECT_REPLY_SEND_DELAY - GetMillisPassed(lastCheckConnectReplySentTimer);
  checkConnectReplyMutex->ReleaseMutex();
  return timeUntil;
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
  pingMutex->WaitOne();
  numSyncRepliesReceived++;
  this->timeOff = (MillisInt)(((float)(numSyncRepliesReceived - 1) / (float)numSyncRepliesReceived) * this->timeOff +
    (1.0f / (float)numSyncRepliesReceived) * timeOff);
  pingMutex->ReleaseMutex();
}

bool ConnectionState::IsSynced()
{
  bool synced = false;
  pingMutex->WaitOne();
  synced = numSyncRepliesReceived >= NUM_SYNC_REPLIES_TO_SYNC;
  pingMutex->ReleaseMutex();
  return synced;
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
  pingMutex->WaitOne();
  if (numSyncRepliesReceived > 0)
  {
    ping = GetTimeSinceStart() - (time + timeOff);
    if (ping < 0) {
      ping = 0;
    }
  }
  pingMutex->ReleaseMutex();
}
PingMillisInt ConnectionState::GetPing()
{
  pingMutex->WaitOne();
  PingMillisInt pingCpy = ping;
  pingMutex->ReleaseMutex();
  return pingCpy;
}
} //namespace constate
} //namespace comnet
