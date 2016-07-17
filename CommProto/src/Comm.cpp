#include "Comm.h"
namespace Comnet {

	Comm::Comm()
	{
	}

	bool Comm::addPacket(AbstractPacket* packet){ return NULL; }

	bool Comm::linkCallback(AbstractPacket* packet, receiver_callback callback){ return NULL; }

	bool Comm::linkQueue(AbstractPacket* packet, Queue<AbstractPacket*>* queue){ return NULL; }


	bool Comm::send(AbstractPacket* packet, uint8_t destId, uint16_t messageId){ return NULL; }

	AbstractPacket* Comm::receive(uint8_t&  sourceId, uint16_t& messageId){ return NULL; }

	int32_t Comm::run(){ return NULL; }

	int32_t Comm::pause(){ return NULL; }


}