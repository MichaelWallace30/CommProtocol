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
private:
		/**
		The amount of milliseconds the ping thread should sleep for when there are no pingers.
		*/
		static const MillisInt EMPTY_SLEEP_TIME_MILLIS = Pinger::PING_TIME_MILLIS;

public:
		/**
		  Creates a new instance of PingManager setting the commsOwner field
				to the argument and links the PingCallaback.
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
				destPingerMapMutex.Lock();
				auto mapIter = destPingerMap.find(destID);
				if (mapIter != destPingerMap.end())
				{
						if (mapIter->second->isInactive())
						{
								activePingersMutex.Lock();
								inactivePingersMutex.Lock();
								TransferToActivePingers(mapIter->second);
								inactivePingersMutex.Unlock();
								activePingersMutex.Unlock();
						}
						else
						{
								activePingersMutex.Lock();
								activePingers.splice(activePingers.end(), activePingers, mapIter->second);  //Move pinger to end of the list because its nextPingTime() has been reset to the maximum value.
								activePingersMutex.Unlock();
						}
						mapIter->second->ResetReceiveTime();
				}
				destPingerMapMutex.Unlock();
		}

		void ResetSendTime(uint8_t destID)
		{
				destPingerMapMutex.Lock();
				auto mapIter = destPingerMap.find(destID);
				if (mapIter != destPingerMap.end()) {
						mapIter->second->ResetSendTime();
				}
				destPingerMapMutex.Unlock();
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
						activePingersMutex.Lock();
						if (!activePingers.empty())	//While there are pingers
						{
								//Starts at the pinger with the lowest NextPingTime and ends once no more pingers need to be sent to (when NextPingTIme is positive)
								//or when the end of the list has been reached.
								auto it = activePingers.begin();
								while (it != activePingers.end())
								{
										//Gets the amount of milliseconds until the pinger needs to be sent to
										MillisInt nextPingTime = it->GetNextPingTimeMillis();
										//When nextPingTime is less than 0 that means its ready to be sent a ping
										if (nextPingTime <= 0)
										{
												SendPingPacket(it->GetDestID());		//Sends a ping packet to the pinger
												it->ResetToResendPingTime();		//Will reset nextPingTime so that it will only be negative after Pinger::PING_RESEND_TIME_MILLIS has passed
												if (it->isInactive()) {
														auto prevIt = it;
														it++;
														inactivePingersMutex.Lock();
														TransferToInactivePingers(prevIt);
														inactivePingersMutex.Unlock();
														continue;
												}
										}
										else
										{
												if (it != activePingers.begin())		//Makes sure that packets actually had their ping time changed
												{
														auto spliceIter = it;	//Iterator representing the position to insert elements into
														while (spliceIter != activePingers.end() && spliceIter->GetNextPingTimeMillis() < Pinger::PING_RESEND_TIME_MILLIS)
														{
																spliceIter++;
														}
														//Puts the pingers that were sento to into the spliceIter position
														activePingers.splice(spliceIter, activePingers, activePingers.begin(), it);
												}
												break;		//Break because we don't need to send a ping to any other packets
										}
										it++;
								}
						}
					 if (activePingers.empty())
						{
								sleepTime = EMPTY_SLEEP_TIME_MILLIS;		//If there are no elements set pingTime to this value.
						}
						else
						{
								sleepTime = activePingers.front().GetNextPingTimeMillis();		//The top element should always have the lowest nextPingTime()
						}
						activePingersMutex.Unlock();
						std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));		//Thread will sleep until a send is needed
				}
		}

		/**
				Creates a new Pinger using the argument and adds it to pingers.
		*/
		void AddPinger(uint8_t destID)
		{
				activePingersMutex.Lock();
			 activePingers.emplace_back(destID);
				activePingersMutex.Unlock();
				destPingerMapMutex.Lock();
				destPingerMap.emplace(std::make_pair(destID, --activePingers.end()));
				destPingerMapMutex.Unlock();
		}

		/**
		  Removes the Pinger with a destID matching the argumeng from pingers.
		*/
		void RemovePinger(uint8_t destID)
		{
				CommLock lock(destPingerMapMutex);
				auto mapIter = destPingerMap.find(destID);
				if (mapIter != destPingerMap.end())
				{
						if (mapIter->second->isInactive())
						{
								inactivePingersMutex.Lock();
								inactivePingers.erase(mapIter->second);
								inactivePingersMutex.Unlock();
						}
						else
						{
								activePingersMutex.Lock();
								activePingers.erase(mapIter->second);
								activePingersMutex.Unlock();
						}
						destPingerMap.erase(mapIter);
				}
		}

		bool CanPong(uint8_t destID)
		{
				CommLock lock(destPingerMapMutex);
				auto mapIter = destPingerMap.find(destID);
				if (mapIter != destPingerMap.end())
				{
						if (mapIter->second->getSendTimePassed() > Pinger::PONG_TIME_MILLIS)
						{
								return true;
						}
				}
				return false;
		}

		void SendPingPacket(uint8_t destID);

		/**
		  Sets running to false allowing the pingSendThread to terminate cleanly.
		*/
  ~PingManager();

private:
		void TransferToActivePingers(std::list <Pinger>::iterator it);

		void TransferToInactivePingers(std::list <Pinger>::iterator it);

		/**
		  Temporary.  Prevents receive thread and ping thread from accessing pingerTimes or destPingerMap at the same time.
		*/
		CommMutex inactivePingersMutex;

		CommMutex activePingersMutex;

		CommMutex destPingerMapMutex;

		/**
		  Holds all of their pingers.  Lowest nextPingTime() is at the beginning, highest nextPingTime() is at the end.
		*/
		std::list <Pinger> activePingers;

		/**
		  Holds Pingers that have been declared as closed after taking too long to respond.
		*/
		std::list <Pinger> inactivePingers;

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
