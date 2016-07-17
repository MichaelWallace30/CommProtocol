#ifndef COMM_H
#define COMM_H
#include <CommProto\CommNode.h>

namespace Comnet {
	class Comm :public CommNode
	{
	public:
		Comm();

		/**
		Polymorphic Destructor.
		*/
		virtual ~Comm()
		{ }
		/**
		Add a packet to the call chain.
		*/
		bool addPacket(AbstractPacket* packet);
		/**
		Link a callback to a packet in the call chain. This is still a working progress, Not sure what
		would be the best thing to accomplish here.
		ex.

		When linking:

		package -> callback -> pair(package,callback) -> map.insert(packageId, pair).

		When Calling:

		package -> map.get(packageId) - return pair -> callback(package_info).

		TODO(): Figure out the best thing for a good callback.
		*/
		bool linkCallback(AbstractPacket* packet, receiver_callback callback);
		/**
		Links the queue of a specified node to a specific queue. Not mandatory, this is optional.
		All packages received will go into a queue anyway.
		*/
		bool linkQueue(AbstractPacket* packet, Queue<AbstractPacket*>* queue);

		// Still in the works...

		/**
		Send the packet to the specified destination address.
		*/
		bool send(AbstractPacket* packet, uint8_t destId, uint16_t messageId);
		/**
		Check for packet if received. This is called manually by user, yet the node should
		be able to run automatically checking for received packets. Any packets linked to a
		callback will be autonomously sent to their respective callback function.
		@param sourceId
		@param messageId
		@return Packet that was received, otherwise NULL if nothing found.
		*/
		AbstractPacket* receive(uint8_t&  sourceId, uint16_t& messageId);

		/**
		Run the node. Threads may be implemented.
		*/
		virtual int32_t run();

		/**
		Pause the node threads and any process.
		*/
		virtual int32_t pause();
	};

}
#endif