#include <CommProto/ping/pingmanager.h>
#include <CommProto/comms.h>

namespace comnet {
namespace ping {
error_t PingCallback(const comnet::Header & header, PingPacket & packet, comnet::Comms & node)
{
		if (packet.isPing())
		{
				if (node.getPingManager()->CanPong(header.source_id))
				{
						PingPacket pingPacket;
						pingPacket.setPing(false);
						node.Send(pingPacket, header.source_id);
				}
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

void PingManager::TransferToActivePingers(std::list<Pinger>::iterator it)
{
		activePingers.splice(activePingers.end(), inactivePingers, it);
		ownerComms->SetActiveState(it->GetDestID(), true);
}

void PingManager::TransferToInactivePingers(std::list<Pinger>::iterator it)
{
		std::cout << "Pinger set to inactive" << std::endl;
		inactivePingers.splice(inactivePingers.end(), activePingers, it);
		ownerComms->SetActiveState(it->GetDestID(), false);
}

} //namespace ping
} //namespace comnet