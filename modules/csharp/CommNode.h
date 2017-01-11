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
#ifndef __CSHARP_COMMNODE_H
#define __CSHARP_COMMNODE_H

#include <CommProto/CommNode.h>

#include <ABSPacket.h>
#include <CallBack.h>
#include <ObjectStream.h>
#include <tools/data_structures/interface/Queue.h>
#include <network/TransportProtocol.h>
#include <pkg/PacketManager.h>
#include <vcclr.h>
#using <mscorlib.dll>

#pragma managed

namespace Comnet {

  using namespace System;
  using namespace Comnet::Serialization;
	using namespace Comnet::Tools::DataStructures::Interface;
  using namespace Comnet::Network;
	using namespace Comnet::Pkg;
  using namespace comnet;

	/**
	CCommNode is the interface used to determine node types that are constructed by the
	library. We use this in extension for our nodes that will be implemented. This will be so
	that we can make easier adjustments for node changing and style, but we must always go
	by this interface if we want to keep our nodes the same.
	*/
	public ref class CommNode abstract {
	public:
		CommNode()
			: uniqueId(number_of_nodes++)
			, nodeId(0)
			, running(false)
			, paused(true)
		{ }

		CommNode(UInt32 platformId)
			: uniqueId(number_of_nodes++)
			, nodeId(platformId)
			, running(false)
			, paused(true)
		{ }
		/**
		Polymorphic Destructor.
		*/
		virtual ~CommNode() 
      { }
		/**
		Add a packet to the call chain.
		*/
		virtual Boolean AddPacket(ABSPacket^ packet) {
      if (!packetManager) {
        throw gcnew System::NullReferenceException("Packet Manager is null! Be sure to initialize first!");
      }

			return this->packetManager->Insert(packet, nullptr);//error need new packet manager
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
		virtual Boolean LinkCallback(ABSPacket^ packet, CallBack^ callback) {
			if (!packetManager) {
        throw gcnew System::NullReferenceException("Packet Manager is null! Be suer to initialize first!");
      }

			return this->packetManager->Insert(packet, callback);//error need new packet manager :(
		}
		/**
		Links the queue of a specified node to a specific queue. Not mandatory, this is optional.
		All packages received will go into a queue anyway.
		*/
		virtual Boolean LinkQueue(const ABSPacket^ packet, Queue<ABSPacket^>^ queue)
		{
			return ~0;
		}

		/**
		Replace the send queue of this node.
		*/
		virtual Boolean ReplaceSendQueue(Queue<ObjectStream^>^ queue) {
			sendQueue = queue;
			return true;
		}

		/**
		Replace the recv queue of this node.
		*/
		virtual Boolean ReplaceReceiveQueue(Queue<ABSPacket^>^ queue) {
			recvQueue = queue;
			return true;
		}
		/**
		Send the packet to the specified destination address.
		*/
		virtual Boolean Send(ABSPacket^ packet, Byte destId) = 0;
		/**
		Check for packet if received. This is called manually by user, yet the node should
		be able to run automatically checking for received packets. Any packets linked to a
		callback will be autonomously sent to their respective callback function.
		@param sourceId
		@param messageId
		@return Packet that was received, otherwise NULL if nothing found.
		*/
		virtual ABSPacket^ Receive(Byte% sourceId) = 0;
		/**
		Initialize connection.
		*/
		virtual bool InitConnection(TransportProtocol connType,
			String^ port,
			String^ address,
			UInt32 baudRate) = 0;//no default arguments allowed maybe overload later

		/**
		Add a communication address.
		*/
		virtual Boolean AddAddress(UInt16 destID, String^ address, UInt16 port) = 0;//no default arguments allowed maybe overload later
		/**
		remove an Address.
		*/
		virtual Boolean RemoveAddress(UInt16 destID) = 0;
		/**
		NOTICE: BE SURE TO CALL THIS METHOD IN YOUR EXTENDED CLASS, IF YOU PLAN ON OVERRIDING THIS
		FUNCTION.

		Run the node. Threads may be implemented.
		*/
		virtual Void Run() {
			running = true;
			paused = false;
		}

		/**
		NOTICE: BE SURE TO CALL THIS METHOD IN YOUR EXTENDED CLASS, IF YOU PLAN ON OVERRIDING THIS
		FUNCTION.

		Pause the node threads and any process.
		*/
		virtual Void Pause() {
			running = false;
			paused = true;
		}

		/**
		NOTICE: BE SURE TO CALL THIS METHOD IN YOUR EXTENDED CLASS, IF YOU PLAN ON OVERRIDING THIS
		FUNCTION.

		Stop the node threads and any process.
		*/
		virtual Void Stop() {
			running = false;
			paused = false;
		}

		/**
		Set the node id.
		*/
		Void SetNodeId(Int32 id)
		{
			this->nodeId = id;
		}

		/**
		Get the node id.
		*/
		Int32 GetNodeId()// const not allowed on managed code
		{
			return this->nodeId;
		}

		/**
		Returns the unique id of this node (the id used for inside communications).
		*/
		Int32 GetUniqueId() //const not allowed on managed code
		{
			return this->uniqueId;
		}

		Boolean IsRunning()
		{
			return running;
		}

		Boolean IsPaused()
		{
			return paused;
		}
	protected:
		/**
		Packet Manager is a manager controller, designed to hold packets for the node.
		*/
    // TODO(Garcia or Wallace) : We need to wrap this.
		PacketManager^ packetManager;//change to managed pointer when new packetmanger is added
		/**
		Queue that holds AbstractPacket types for receiving.
		*/
		Queue<ABSPacket^>^ recvQueue;
		/**
		Queue that holds ObjectStreamWrappers used for sending out.
		*/
		Queue<ObjectStream^>^ sendQueue;
	private:
		/**
		The node id associated with this node. This id is used for outside communications.
		Be sure to set an id that would most likely be identifiable with your node.
		*/
		UInt32 nodeId;
		/**
		The unique id used for inside communications. This id CANNOT be changed.
		*/
		UInt32 uniqueId;
		/**
		Check if this node is currently running.
		*/
		Boolean running;
		/**
		Check if ths node is paused.
		*/
		Boolean paused;
	};

} // Comnet namespace

#endif // __CSHARP_COMMNODE_H
