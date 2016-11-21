#include <CommProto/ping/pinger.h>
#include <iostream>

namespace comnet {
namespace ping {

Pinger::Pinger(uint8_t destID)
		:destID(destID)
{
		ResetReceiveTime();
}

void Pinger::ResetToResendPingTime()
{
		CommLock lock(receiveTimeMutex);
		lastPingTime = Time::now();	//Set to current time
		pingTime = PING_RESEND_TIME_MILLIS;  //We should now be sending the packet more often, so set pingTime to Pinger::PING_RESEND_TIME_MILLIS.
}

void Pinger::ResetReceiveTime()
{
		CommLock lock(receiveTimeMutex);
		lastReceiveTime = Time::now();  //Set to current time
		lastPingTime = Time::now();  //Set to current time
		pingTime = PING_TIME_MILLIS;  //Now that we have received a packet, we no longer have to resend the pingpacket more often so set pingTime back to PING_TIME_MILLIS
}

MillisInt Pinger::GetNextPingTimeMillis()
{
		CommLock lock(receiveTimeMutex);
	 fsec fs = Time::now() - lastPingTime;	//amount of time that has passed since lastPingTime
		ms d = std::chrono::duration_cast<ms>(fs);		//converting time to milliseconds
		return pingTime - d.count();
}

Pinger::~Pinger()
{
}

} //namespace ping
} //namespace comnet
