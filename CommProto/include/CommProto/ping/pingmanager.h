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
#include <CommProto/tools/data_structures/thread_safe_list.h>
#include <CommProto/architecture/os/comm_mutex.h>
#include <CommProto/architecture/os/comm_thread.h>
#include <thread>
#include <unordered_map>
#include <memory>
#include <iostream>

namespace comnet {

class Comms;

namespace ping {

using namespace comnet::tools::datastructures;

/**
  The callback linked to the pingPacket.  Will call.
*/
error_t PingCallback(const comnet::Header& header, PingPacket& packet, comnet::Comms& node);

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
		bool Run()
		{
				running = true;
				pingSendThread = std::make_shared <CommThread>(&PingManager::HandlePingUpdate, shared_from_this());
				pingSendThread->Detach();
				return true;
		}

		/**
		  Resets the nextPingerTime() on the pinger with the same destID as the argument.
				Moves the pinger to the end of the pingerTimes list.
		*/
		void ResetPingTime(uint8_t destID)
		{
				pingerMutex.lock();
				auto mapIter = destPingerMap.find(destID);
				if (mapIter != destPingerMap.end())
				{
						mapIter->second->ResetReceiveTime();
						pingerTimes.splice(pingerTimes.end(), pingerTimes, mapIter->second);  //Move pinger to end of the list because its nextPingTime() has been reset to the maximum value.
				}
				pingerMutex.unlock();
		}

		/**
		  The method run by the pingSendThread.  Checks if a pingPacket
				needs to be sent to any element of pinger.  Will also close the connection
				if enough time has passed without receiving a packet from a client.
		*/
		void HandlePingUpdate()
		{
				while (true)
				{
						runningMutex.Lock();
						//Checks if the thread should still be running
						if (!running)
						{
								runningMutex.Unlock();
								return;
						}
						runningMutex.Unlock();
						//The amount of milliseconds the thread should sleep for after finished
						MillisInt sleepTime;
						pingerMutex.lock();
						if (!pingerTimes.empty())	//While there are pingers
						{
								//Starts at the pinger with the lowest NextPingTime and ends once no more pingers need to be sent to (when NextPingTIme is positive)
								//or when the end of the list has been reached.
								for (auto it = pingerTimes.begin(); it != pingerTimes.end(); it++)
								{
										//Gets the amount of milliseconds until the pinger needs to be sent to
										MillisInt nextPingTime = it->GetNextPingTimeMillis();
										//WHen nextPingTIme is less than 0 that means its ready to be sent a ping
										if (nextPingTime <= 0)
										{
												SendPingPacket(it->GetDestID());		//Sends a ping packet to the pinger
												it->ResetToResendPingTime();		//Will reset nextPingTime so that it will only be negative after Pinger::PING_RESEND_TIME_MILLIS has passed
										}
										else
										{
												if (it != pingerTimes.begin())		//If we've ResetToResendPingTime() any packets, this needs to be called.
												{
														auto spliceIter = it;	//Iterator representing the position to insert elements into
														while (spliceIter != pingerTimes.end() && spliceIter->GetNextPingTimeMillis() < Pinger::PING_RESEND_TIME_MILLIS)
														{
																spliceIter++;
														}
														//Puts the pingers that were sento to into the spliceIter position
														pingerTimes.splice(spliceIter, pingerTimes, pingerTimes.begin(), it);
												}
												break;		//Break because we don't need to send a ping to any other packets
										}
								}
								sleepTime = pingerTimes.front().GetNextPingTimeMillis();		//The top element should always have the lowest nextPingTime()
						}
						else
						{
								sleepTime = EMPTY_SLEEP_TIME_MILLIS;		//If there are no elements set pingTime to this value.
						}
						pingerMutex.unlock();
						std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));		//Thread will sleep until a send is needed
				}
		}

		/**
				Creates a new Pinger using the argument and adds it to pingers.
		*/
		void AddPinger(uint8_t destID)
		{
				pingerMutex.lock();
				pingerTimes.emplace_back(destID);
				destPingerMap.emplace(std::make_pair(destID, --pingerTimes.end()));
				pingerMutex.unlock();
		}

		/**
		  Removes the Pinger with a destID matching the argumeng from pingers.
		*/
		void RemovePinger(uint8_t destID)
		{
				pingerMutex.lock();
				auto mapIter = destPingerMap.find(destID);
				if (mapIter != destPingerMap.end())
				{
						pingerTimes.erase(mapIter->second);
						destPingerMap.erase(mapIter);
				}
				pingerMutex.unlock();
		}

		void SendPingPacket(uint8_t destID);

		/**
		  Sets running to false allowing the pingSendThread to terminate cleanly.
		*/
  ~PingManager();

private:
		/**
		  Temporary.  Prevents receive thread and ping thread from accessing pingerTimes or destPingerMap at the same time.
		*/
		std::mutex pingerMutex;

		/**
		  The amount of milliseconds the ping thread should sleep for when there are no pingers.
		*/
		static const MillisInt EMPTY_SLEEP_TIME_MILLIS = Pinger::PING_TIME_MILLIS;

		/**
		  Holds all of their pingers.  Lowest nextPingTime() is at the beginning, highest nextPingTime() is at the end.
		*/
		std::list <Pinger> pingerTimes;

		/**
		  The key is the destination id of the Pinger.  When a packet is received, it will use the dest_id to reset the pinger and move the
				list iterator to the end of pingerTimes.
		*/
		std::unordered_map <uint8_t, std::list <Pinger>::iterator> destPingerMap;

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
