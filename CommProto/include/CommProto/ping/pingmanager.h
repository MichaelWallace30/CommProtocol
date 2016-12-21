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
  The callback linked to {@link #pingPacket}.  Checks whether the {@link PingPacket} is a ping
  and if a pong should be sent back.
  @param header The header data of the packet, only used field is source_id.
  @param packet The {@link PingPacket} that will either be a Ping or a Pong.
  @param node The {@link Comms} that owns the {@link PingManager} that linked the callback.
  @return Tells {@link Comms} how the packet should be finalized.
*/
error_t PingCallback(const comnet::Header& header, PingPacket& packet, comnet::Comms& node);

/**
  Manages {@link Pinger}s and handles the sending and receiving of {@link PingPacket}s
*/
class PingManager : public std::enable_shared_from_this <PingManager>
{
private:
  /**
    The amount of milliseconds the {@link #pingSendThread} should sleep for when there are no 
    elements in {@link #activePingers}.
  */
  static const MillisInt EMPTY_SLEEP_TIME_MILLIS = Pinger::PING_TIME_MILLIS;

public:
  /**
    Creates a new instance of {@link PingManager} setting the commsOwner field
    to the argument and links the PingCallaback.
    @param commsOwner The {@link Comms} that owns {@code this}.
    @return An instance of {@link PingManager}.
  */
  PingManager(Comms* commsOwner);

  /**
    Links {@link PingCallback} to {@link #pingPacket} in the {@link #ownerComms}.
  */
  void LinkPingCallback();

  /**
    Initializes and detaches the pingSendThread.
    {@code true} when successfully running, {@code false} otherwise.
  */
  bool Run()
  {
    running = true;
    pingSendThread = std::make_shared <CommThread>(&PingManager::HandlePingUpdate, shared_from_this());
    pingSendThread->Detach();
    return true;
  }

  /**
    Checks if the {@link Pinger} that matches the destID is active.
    @param destID The node_id of to check.
    @return {@code true} if active, {@code false} if inactive or not found.
  */
  bool IsActive(uint8_t destID)
  {
    CommLock lock(destPingerMapMutex);
    auto mapIt = destPingerMap.find(destID);
    if (mapIt != destPingerMap.end())
    {
      return !mapIt->second->IsInactive();
    }
    return false;
  }

  /**
    Resets the {@link Pinger#nextPingerTime()} on the {@link Pinger} with the same id as the argument.
    In other words, it moves the corresponding {@link Pinger} to the end of {@link #activePingers}.
    @param The node_id of the {@link Pinger} to reset.
  */
  void ResetPingTime(uint8_t destID)
  {
    destPingerMapMutex.Lock();
    auto mapIter = destPingerMap.find(destID);
    if (mapIter != destPingerMap.end())
    {
      if (mapIter->second->IsInactive())
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

  /**
    Resets the {@link Pinger#GetSendTimePassed} of the {@link Pinger} with the id
    of the argument.
    @param destID The node_id of the {@link Pinger} to reset the send time of.
  */
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
    The method run by the {@link #pingSendThread}.  Checks if a {@link PingPacket}
    needs to be sent to any element of {@link #activePingers}.  Will also deactivate
    {@link Pinger}s if enough time has passed without receiving a packet from a client.
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
            if (it->IsInactive()) {
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
    Creates a new Pinger using the argument and adds it to {@link #activePingers}
    and {@link #destPingerMap}.
    @param destID The remote node_id that the {@link Pinger} will be bound to.
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
    Removes the {@link Pinger} with a destID matching the argument.
    @param destID The remote node_id of the {@link Pinger} that should be removed.
  */
  void RemovePinger(uint8_t destID)
  {
    CommLock lock(destPingerMapMutex);
    auto mapIter = destPingerMap.find(destID);
    if (mapIter != destPingerMap.end())
    {
      if (mapIter->second->IsInactive())
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

  /**
    Checks if enough time has passed since the last time a packet was sent
    to the remote node_id for a Pong to be sent back. This avoids unecessarily
    sending more packets.
    @param The id of the remote comms node that should be checked.
    @return {@code true} if a Pong should be sent, {@code false} otherwise.	
 */
  bool CanPong(uint8_t destID)
  {
    CommLock lock(destPingerMapMutex);
    auto mapIter = destPingerMap.find(destID);
    if (mapIter != destPingerMap.end())
    {
      if (mapIter->second->GetSendTimePassed() > Pinger::PONG_TIME_MILLIS)
      {
        return true;
      }
    }
    return false;
  }

  /**
    Sends a {@link PingPacket} to the specified destID.
    @param The id of the remote node to send the ping to.
  */
  void SendPingPacket(uint8_t destID);

  /**
    Sets {@link #running} to {@code false} allowing the {@link #pingSendThread} to terminate cleanly.
  */
  ~PingManager();

private:
  /**
    Called when a packet was received from a {@link Pinger} that was
    considered inactive. The method will move the specified {@link Pinger}
    from {@link #inactivePingers} to {@link #activePingers}.
    @param it The iterator pointing to the {@link Pinger} that should be transfered.
  */
  void TransferToActivePingers(std::list <Pinger>::iterator it);

  /**
    Called when {@link Pinger#isInactive} is {@code true}. Will move
    the {@link Pinger} from {@link #activePingers} to {@link #inactivePingers}
    so it will no longer be sent anymore {@link PingPacket}s.
    @param it The iterator pointing to the {@link Pinger} that should be transfered.
  */
  void TransferToInactivePingers(std::list <Pinger>::iterator it);

  /**
    Prevents threads from reading and modifying {@link #inactivePingers} at the same time.
  */
  CommMutex inactivePingersMutex;

  /**
    Prevents threads from reading and modifying {@link #activePingers} at the same time.
  */
  CommMutex activePingersMutex;

  /**
    Prevents threads from reading and modifying {@link #destPingerMapMutex} at the same time.
  */
  CommMutex destPingerMapMutex;

  /**
    Holds all of their pingers.  Lowest nextPingTime() is at the beginning, highest nextPingTime() is at the end.
  */
  std::list <Pinger> activePingers;

  /**
    Holds {@link Pinger}s that have been declared as inactive after taking too long to respond.
  */
  std::list <Pinger> inactivePingers;

  /**
    The key is the destination id of the {@link Pinger}.  When a packet is received, it will use the dest_id to access
    the corresponding {@link Pinger}.
  */
  std::unordered_map <uint8_t, std::list <Pinger>::iterator> destPingerMap;

  /**
    The thread that runs the {@link #HandlePingUpdate} method.  Will shut down once
    {@link #running} is set to {@code false}.
  */
  std::shared_ptr<CommThread> pingSendThread;

  /**
    Prevents {@link #running} from being modified at the same time {@link #HandlePingUpdate}
    is checking it.
  */
  CommMutex runningMutex;

  /**
    The packet used for linking the callback in {@link #LinkPingCallback}.
  */
  PingPacket* pingPacket;

  /**
    The {@link Comms} object that owns {@code this}.
  */
  Comms* ownerComms;

  /**
    {@code true} when the {@link #pingSendThread} should be running, {@code false} when the thread
    should stop running.
  */
  bool running;
};
} // namespace ping
}  // namespace comnet
#endif  //__PING_MANAGER_H
