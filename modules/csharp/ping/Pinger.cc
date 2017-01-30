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

#include <ping/Pinger.h>
#include <iostream>

namespace Comnet {
  namespace Ping {
    using namespace System;

    Pinger::Pinger(uint8_t destID)
      :destID(destID), pingAttempts(0)
    {
      lastPingTime = gcnew Diagnostics::Stopwatch();
      lastSendTime = gcnew Diagnostics::Stopwatch();
						lastSyncPackSentTime = gcnew Diagnostics::Stopwatch();
      pingTimeMutex = gcnew Threading::Mutex();
      sendTimeMutex = gcnew Threading::Mutex();
      pingAttemptsMutex = gcnew Threading::Mutex();
						pingMutex = gcnew Threading::Mutex();
						syncMutex = gcnew Threading::Mutex();
      ResetReceiveTime();
      ResetSendTime();
    }

				Void Pinger::ResetSyncPackSentTime()
				{
						syncMutex->WaitOne();
						lastSyncPackSentTime->Restart();
						syncSendDelay = SYNC_PACK_SEND_MILLIS;
						syncMutex->ReleaseMutex();
				}

    Void Pinger::ResetToResendPingTime()
    {
      pingTimeMutex->WaitOne();
      lastPingTime->Restart();
      pingTime = PING_RESEND_TIME_MILLIS;  //We should now be sending the packet more often, so set pingTime to Pinger::PING_RESEND_TIME_MILLIS.
      pingTimeMutex->ReleaseMutex();
      pingAttemptsMutex->WaitOne();
      pingAttempts++;
      pingAttemptsMutex->ReleaseMutex();
    }

    Void Pinger::ResetReceiveTime()
    {
      pingTimeMutex->WaitOne();
      lastPingTime->Restart();
      pingTime = PING_TIME_MILLIS;  //Now that we have received a packet, we no longer have to resend the pingpacket more often so set pingTime back to PING_TIME_MILLIS
      pingTimeMutex->ReleaseMutex();
      pingAttemptsMutex->WaitOne();
      pingAttempts = 0;
      pingAttemptsMutex->ReleaseMutex();
    }

    Void Pinger::ResetSendTime()
    {
      sendTimeMutex->WaitOne();
      lastSendTime->Restart();
      sendTimeMutex->ReleaseMutex();
    }

				Void Pinger::SetAsUnsynced()
				{
						syncMutex->WaitOne();
						numSyncPackReplysReceived = 0;
						syncMutex->ReleaseMutex();
				}

				Boolean Pinger::IsSynced()
				{
						syncMutex->WaitOne();
						bool isSynced = numSyncPackReplysReceived >= NUM_SYNC_PACKS;
						syncMutex->ReleaseMutex();
						return isSynced;
				}

				Void Pinger::SyncTime(int32_t timeOff)
				{
						syncMutex->WaitOne();
						numSyncPackReplysReceived++;
						timeOffMillis = (MillisInt)(((float)(numSyncPackReplysReceived - 1) / (float)numSyncPackReplysReceived) * this->timeOffMillis + (1.0f / (float)numSyncPackReplysReceived) * timeOff);
						syncMutex->ReleaseMutex();
				}

				Void Pinger::ResetPing(int32_t time)
				{
						syncMutex->WaitOne();
						if (numSyncPackReplysReceived > 0)
						{
								ping = GetTimeSinceStart() - (time + timeOffMillis);
								std::cout << "PING: " << ping << std::endl;
								if (ping < 0)
								{
										ping = 0;
								}
						}
						syncMutex->ReleaseMutex();
				}

    MillisInt Pinger::GetNextPingTimeMillis()
    {
      pingTimeMutex->WaitOne();
      MillisInt nextPingTime = pingTime - GetMillisPassed(lastPingTime);
      pingTimeMutex->ReleaseMutex();
      return nextPingTime;
    }

				MillisInt Pinger::GetNextSyncTimeMillis()
				{
						syncMutex->WaitOne();
						MillisInt nextSyncTime = syncSendDelay - GetMillisPassed(lastSyncPackSentTime);
						syncMutex->ReleaseMutex();
						return nextSyncTime;
				}
  } //namespace ping
} //namespace comnet
