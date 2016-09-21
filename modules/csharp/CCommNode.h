/*
CCommNode interface for modular node connection.

Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/connection/transport_type.h>
#include <CommProto/pkg/PacketManager.h>//need new packet manager?
#include <CommProto/tools/data_structures/interface/InterfaceQueue.h>

#pragma once
#pragma managed

namespace Comnet {


	using namespace Comnet::Tools::DataStructures::Interface;
	using namespace Comnet::Pkg;

	ref class AbstractPacketForwarder;//I assume this will change
	ref class CallBack;
	ref class ObjectStreamWrapper;

	/**
	Currently supports about 65,536 nodes.
	*/
	static uint16_t numberOfNodes = 0;
	/**
	CCommNode is the interface used to determine node types that are constructed by the
	library. We use this in extension for our nodes that will be implemented. This will be so
	that we can make easier adjustments for node changing and style, but we must always go
	by this interface if we want to keep our nodes the same.
	*/
	ref class CCommNode abstract{
	public:
		CCommNode()
			: uniqueId(numberOfNodes++)
			, nodeId(0)
			, running(false)
			, paused(true)
		{ }

		CCommNode(const uint32_t platformId)
			: uniqueId(numberOfNodes++)
			, nodeId(platformId)
			, running(false)
			, paused(true)
		{ }
		/**
		Polymorphic Destructor.
		*/
		virtual ~CCommNode() {
			free_pointer(recvQueue);
			free_pointer(sendQueue);
		}
		/**
		Add a packet to the call chain.
		*/
		virtual bool addPacket(const AbstractPacketForwarder^ packet) {
			return this->packetManager->insert(packet, NULL);//error need new packet manager
		}
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
		virtual bool linkCallback(const AbstractPacketForwarder^ packet, const CallBack^ callback) {
			
			return this->packetManager->insert(packet, callback);//error need new packet manager :(
		}
		/**
		Links the queue of a specified node to a specific queue. Not mandatory, this is optional.
		All packages received will go into a queue anyway.
		*/
		virtual bool linkQueue(const AbstractPacketForwarder^ packet, const Queue<AbstractPacketForwarder^>* queue)
		{
			return ~0;
		}

		/**
		Replace the send queue of this node.
		*/
		virtual bool replaceSendQueue(const Queue<ObjectStreamWrapper^>* queue) {
			free_pointer(sendQueue);
			sendQueue = (Queue<ObjectStreamWrapper^> *) queue;
			return true;
		}

		/**
		Replace the recv queue of this node.
		*/
		virtual bool replaceReceiveQueue(const Queue<AbstractPacketForwarder^>* queue) {
			free_pointer(recvQueue);
			recvQueue = (Queue<AbstractPacketForwarder^> *) queue;
			return true;
		}
		/**
		Send the packet to the specified destination address.
		*/
		virtual bool send(AbstractPacketForwarder^ packet, uint8_t destId) = 0;
		/**
		Check for packet if received. This is called manually by user, yet the node should
		be able to run automatically checking for received packets. Any packets linked to a
		callback will be autonomously sent to their respective callback function.
		@param sourceId
		@param messageId
		@return Packet that was received, otherwise NULL if nothing found.
		*/
		virtual AbstractPacketForwarder^ receive(uint8_t& sourceId) = 0;
		/**
		Initialize connection.
		*/
		virtual bool initConnection(transport_protocol_t connType,
			const char* port,
			const char* address,
			uint32_t baudRate) = 0;//no default arguments allowed maybe overload later

		/**
		Add a communication address.
		*/
		virtual bool addAddress(uint8_t destID, const char* address, uint16_t port) = 0;//no default arguments allowed maybe overload later
		/**
		remove an Address.
		*/
		virtual bool removeAddress(uint8_t destID) = 0;
		/**
		NOTICE: BE SURE TO CALL THIS METHOD IN YOUR EXTENDED CLASS, IF YOU PLAN ON OVERRIDING THIS
		FUNCTION.

		Run the node. Threads may be implemented.
		*/
		virtual int32_t run() {
			running = true;
			paused = false;

			return 0;
		}

		/**
		NOTICE: BE SURE TO CALL THIS METHOD IN YOUR EXTENDED CLASS, IF YOU PLAN ON OVERRIDING THIS
		FUNCTION.

		Pause the node threads and any process.
		*/
		virtual int32_t pause() {
			running = false;
			paused = true;

			return 0;
		}

		/**
		NOTICE: BE SURE TO CALL THIS METHOD IN YOUR EXTENDED CLASS, IF YOU PLAN ON OVERRIDING THIS
		FUNCTION.

		Stop the node threads and any process.
		*/
		virtual int32_t stop() {
			running = false;
			paused = false;

			return 0;
		}

		/**
		Set the node id.
		*/
		void setNodeId(int32_t id)
		{
			this->nodeId = id;
		}

		/**
		Get the node id.
		*/
		int32_t getNodeId()// const not allowed on managed code
		{
			return this->nodeId;
		}

		/**
		Returns the unique id of this node (the id used for inside communications).
		*/
		int32_t getUniqueId() //const not allowed on managed code
		{
			return this->uniqueId;
		}

		bool isRunning()
		{
			return running;
		}

		bool isPaused()
		{
			return paused;
		}
	protected:
		/**
		Packet Manager is a manager controller, designed to hold packets for the node.
		*/
		PacketManager *packetManager;//change to managed pointer when new packetmanger is added
		/**
		Queue that holds AbstractPacket types for receiving.
		*/
		Queue<AbstractPacketForwarder^>* recvQueue;
		/**
		Queue that holds ObjectStreamWrappers used for sending out.
		*/
		Queue<ObjectStreamWrapper^>* sendQueue;
	private:
		/**
		The node id associated with this node. This id is used for outside communications.
		Be sure to set an id that would most likely be identifiable with your node.
		*/
		int32_t nodeId;
		/**
		The unique id used for inside communications. This id CANNOT be changed.
		*/
		const int32_t uniqueId;
		/**
		Check if this node is currently running.
		*/
		bool running;
		/**
		Check if ths node is paused.
		*/
		bool paused;
	};

} // Comnet namespace


