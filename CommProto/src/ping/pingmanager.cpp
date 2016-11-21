#include <CommProto/ping/pingmanager.h>

namespace comnet {
namespace ping {

		PingManager::PingManager(Comms* ownerComms)
				:std::enable_shared_from_this <PingManager>(), ownerComms(ownerComms)
{
}

bool PingManager::Init()
{
		return false;
}

void PingManager::HandlePingUpdate()
{
}

void PingManager::AddPinger(uint8_t destID)
{
}

void PingManager::RemovePinger(uint8_t destID)
{
}

PingManager::~PingManager()
{
}

} //namespace ping
} //namespace comnet