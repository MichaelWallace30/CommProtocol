/*
  Runs a thread to update pingers.

  Copyright (C) 2016  Alex Craig, Michael Wallace, Mario Garcia.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (At your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __PING_MANAGER_H
#define __PING_MANAGER_H

#include <CommProto/ping/pingpacket.h>
#include <CommProto/ping/pinger.h>
#include <CommProto/callback.h>
#include <CommProto/comms.h>
#include <CommProto/tools/data_structures/thread_safe_list.h>
#include <CommProto/architecture/os/comm_mutex.h>
#include <CommProto/architecture/os/comm_thread.h>
#include <memory>

namespace comnet {
namespace ping {

using namespace comnet::tools::datastructures;

/**
  The callback linked to the pingPacket.  Will call 
*/
error_t PingCallback(const comnet::Header& header, const PingPacket& packet, comnet::Comms& node)
{

}

/**
  Manages the Pingers and does the sending and receiving of PingPackets
*/
class PingManager : public std::enable_shared_from_this <PingManager>
{
public:
		/**
		  Creates a new instance of PingManager setting commsOwner
				to the argument.
		*/
  PingManager(Comms* commsOwner);

		/**
		  Initializes and detaches the pingSendThread.
		*/
		bool Init();

		/**
		  The method run by the pingSendThread.  Checks if a pingPacket
				needs to be sent to any element of pinger.  Will also close the connection
				if enough time has passed without receiving a packet from a client.
		*/
		void HandlePingUpdate();

		/**
				Creates a new Pinger using the argument and adds it to pingers.
		*/
		void AddPinger(uint8_t destID);

		/**
		  Finds the Pinger with the same destID. Returns nullptr if not found.
		*/
		Pinger* GetPinger(uint8_t destID)
		{

		}

		/**
		  Removes the Pinger with a destID matching the argumeng from pingers.
		*/
		void RemovePinger(uint8_t destID);

		/**
		  Sets running to false allowing the pingSendThread to terminate cleanly.
		*/
  ~PingManager();

private:
		/**
		  This will probably change... I think an unordered_map might be better.
				But whatever it is, we need to make it thread safe.
				
				Stores the pingers, the Pingers are ordered by the amount of time
				until they need to be pinged from lowest to highest.
		*/
		ThreadSafeList <Pinger*> pingers;

		/**
		  The thread that runs the HandlePingUpdate() method.  Will shut down once
				running is set to false.
		*/
		std::shared_ptr<CommThread> pingSendThread;

		/**
				Prevents running from being modified at the same time HandlePingUpdate()
				is checking it.
		*/
		CommMutex runningMutex;

		/**
		  The packet used for sending pings. Can we use one packet for all pinging?
				Or do we have to make a new one each time?
		*/
		PingPacket* pingPacket;

		/**
		  The Comms object that owns this PingManager.
		*/
		Comms* ownerComms;

		/**
		  True when the sendThread should be running, false otherwise.
		*/
		bool running;
};
} // namespace ping
}  // namespace comnet
#endif  //__PING_MANAGER_H
