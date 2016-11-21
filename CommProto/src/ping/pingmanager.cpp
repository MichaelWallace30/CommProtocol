#include <CommProto/ping/pingmanager.h>
#include <CommProto/comms.h>

namespace comnet {
namespace ping {
error_t PingCallback(const comnet::Header & header, PingPacket & packet, comnet::Comms & node)
{
		if (packet.isPing())
		{
				std::cout << "PING RECEIVED" << std::endl;
				PingPacket pingPacket;
				pingPacket.setPing(false);
				node.Send(pingPacket, header.source_id);
		}
		else
		{
				std::cout << "PONG RECEIVED" << std::endl;
		}
		return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
}

PingManager::PingManager(Comms* ownerComms)
		:std::enable_shared_from_this <PingManager>(), ownerComms(ownerComms)
{
				pingPacket = new PingPacket();
				ownerComms->LinkCallback(pingPacket, new Callback((comnet::callback_t)PingCallback));
}

void PingManager::SendPingPacket(uint8_t destID)
{
		PingPacket sendPacket;
		sendPacket.setPing(true);
		ownerComms->Send(sendPacket, destID);
}

PingManager::~PingManager()
{
		runningMutex.Lock();
		running = false;
		runningMutex.Unlock();
}

} //namespace ping
} //namespace comnet