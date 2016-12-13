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
