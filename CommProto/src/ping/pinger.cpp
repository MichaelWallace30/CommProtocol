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

const TimePoint Pinger::START_TIME = Pinger::GetNow();

Pinger::Pinger(uint8_t destID)
  :destID(destID), pingAttempts(0), numSyncPackReplysReceived(0), ping(-1), timeOffMillis(0), syncSendDelay(-1)
{
		CommLock pingLock(pingTimeMutex);
		lastPingTime = GetNow();  //Set to current time
		pingTime = 0;  //Now that we have received a packet, we no longer have to resend the pingpacket more often so set pingTime back to PING_TIME_MILLIS
		CommLock attemptLock(pingAttemptsMutex);
		pingAttempts = 0;
		ResetSendTime();
}

void Pinger::ResetSyncPackSentTime()
{
		CommLock lock(syncMutex);
		lastSyncPackSentTime = GetNow();
		syncSendDelay = SYNC_PACK_SEND_MILLIS;
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

bool Pinger::IsSynced()
{
		CommLock syncLock(syncMutex);
		return numSyncPackReplysReceived >= NUM_SYNC_PACKS;
}

bool Pinger::CheckResync(int64_t unixHighResTimeDif)
{
		CommLock lock(syncMutex);
		return numSyncPackReplysReceived > 0 && abs(this->unixHighResTimeDif - unixHighResTimeDif) > MAX_CLOCK_DIF;
}

void Pinger::Resync()
{
		CommLock syncLock(syncMutex);
		CommLock pingLock(pingMutex);
		ping = -1;
		numSyncPackReplysReceived = 0;
}

void Pinger::SyncTime(int32_t timeOff)
{
		CommLock syncLock(syncMutex);
		numSyncPackReplysReceived++;
		timeOffMillis = (MillisInt)(((float)(numSyncPackReplysReceived - 1) / (float)numSyncPackReplysReceived) * this->timeOffMillis + (1.0f / (float)numSyncPackReplysReceived) * timeOff);
}

void Pinger::ResetPing(int32_t time)
{
		CommLock syncLock(syncMutex);
		if (numSyncPackReplysReceived > 0)
		{
				ping = GetTimeSinceStart() - (time + timeOffMillis);
				if (ping < 0)
				{
						ping = 0;
				}
				std::cout << "PING: " << ping << std::endl;
		}
}

MillisInt Pinger::GetNextPingTimeMillis()
{
		CommLock pingTimeLock(pingTimeMutex);
	 return pingTime - GetMillisPassed(lastPingTime);
}

MillisInt Pinger::GetNextSyncTimeMillis()
{
		CommLock syncLock(syncMutex);
		return syncSendDelay - GetMillisPassed(lastSyncPackSentTime);
}

Pinger::~Pinger()
{
}

} //namespace ping
} //namespace comnet
