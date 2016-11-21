#include <CommProto/ping/pinger.h>

namespace comnet {
namespace ping {

Pinger::Pinger(uint8_t destID)
		:destID(destID)
{
}

void Pinger::ResetReceiveTime()
{
}

MillisInt Pinger::GetNextPingTimeMillis()
{
		return MillisInt();
}

Pinger::~Pinger()
{
}

} //namespace ping
} //namespace comnet
