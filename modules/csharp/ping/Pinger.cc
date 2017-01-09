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
						pingTimeMutex = gcnew Threading::Mutex();
						sendTimeMutex = gcnew Threading::Mutex();
						pingAttemptsMutex = gcnew Threading::Mutex();
						ResetReceiveTime();
						ResetSendTime();
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

				void Pinger::ResetSendTime()
				{
						sendTimeMutex->WaitOne();
						lastSendTime->Restart();
						sendTimeMutex->ReleaseMutex();
				}

				MillisInt Pinger::GetNextPingTimeMillis()
				{
						pingTimeMutex->WaitOne();
						MillisInt nextPingTime = pingTime - GetMillisPassed(lastPingTime);
						pingTimeMutex->ReleaseMutex();
						return nextPingTime;
				}

		} //namespace ping
} //namespace comnet
