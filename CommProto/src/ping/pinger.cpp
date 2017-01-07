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

#include <CommProto/ping/pinger.h>
#include <iostream>

namespace comnet {
namespace ping {

Pinger::Pinger(uint8_t destID)
  :destID(destID), pingAttempts(0)
{
  ResetReceiveTime();
}

void Pinger::ResetToResendPingTime()
{
  CommLock pingLock(pingTimeMutex);
  lastPingTime = GetNow();	//Set to current time
  pingTime = PING_RESEND_TIME_MILLIS;  //We should now be sending the packet more often, so set pingTime to Pinger::PING_RESEND_TIME_MILLIS.
  CommLock attemptLock(pingAttemptsMutex);
  pingAttempts++;
}

void Pinger::ResetReceiveTime()
{
  CommLock pingLock(pingTimeMutex);
  lastPingTime = GetNow();  //Set to current time
  pingTime = PING_TIME_MILLIS;  //Now that we have received a packet, we no longer have to resend the pingpacket more often so set pingTime back to PING_TIME_MILLIS
  CommLock attemptLock(pingAttemptsMutex);
  pingAttempts = 0;
}

void Pinger::ResetSendTime()
{
  CommLock lock(sendTimeMutex);
  lastSendTime = GetNow();
}

MillisInt Pinger::GetNextPingTimeMillis()
{
  CommLock lock(pingTimeMutex);
  return pingTime - GetMillisPassed(lastPingTime);
}

Pinger::~Pinger()
{
}

} //namespace ping
} //namespace comnet
