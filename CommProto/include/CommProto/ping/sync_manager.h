/*
Runs a thread to sync time between pingers.

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

#ifndef __SYNC_MANAGER_H
#define __SYNC_MANAGER_H

#include <CommProto/callback.h>
#include <CommProto/architecture/os/comm_thread.h>
#include <CommProto/architecture/os/comm_mutex.h>

#include <mutex>
#include <list>
#include <memory>

namespace comnet
{
  class Comms;
namespace ping
{
  using namespace comnet::architecture::os;

  class Pinger;
  class SyncRequestPacket;
  class SyncReplyPacket;

  /**
  The callback linked to {@link #syncRequestPacket}. Will reply with a {@link #syncReplyPacket}.
  @param header The header data of the packet, only used field is source_id.
  @param packet The {@link SyncRequestPacket}.
  @param node The {@link Comms} that owns the {@link PingManager} that linked the callback.
  @return Tells {@link Comms} how the packet should be finalized.
  */
  error_t SyncRequestCallback(const comnet::Header& header, SyncRequestPacket& packet, comnet::Comms& node);

  /**
  The callback linked to {@link #syncReplyPacket}.  Will call {@link #syncTime} on the pinger that sent the reply.
  */
  error_t SyncReplyCallback(const comnet::Header& header, SyncReplyPacket& packet, comnet::Comms& node);
  
  /**
  Runs a thread to sync time between {@link Pinger}s.
  */
  class SyncManager : public std::enable_shared_from_this<SyncManager>
  {
  public:
    /**
    Creates a new instance of {@link SyncManager}.
    Initializes {@link #syncRequestPacket} and {@link #syncReplyPacket}.
    */
    SyncManager(Comms* owner);

    /**
    The method run by {@link #syncSendThread}.
    Checks if a {@link #SyncRequestPacket} needs to be sent.
    */
    void SyncSendHandler();

    /**
    Adds a {@link Pinger} to {@link #unsyncedPingers} so that the
    {@link #SyncSendHandler} can send messages to it.
    */
    void AddUnsyncedPinger(Pinger* pinger);

    /**
    Called when {#link Pinger} is inactive, removed, or done syncing.
    @link pinger The {@link Pinger} to be removed from {@link #unsyncedPingers}.
    */
    void RemoveUnsyncedPinger(Pinger* pinger);

    /**
    Called by {@link #SyncReplyCallback}.  Sets the timestamp of the peer.
    @param pinger The {@link Pinger} to adjust the time on.
    @param time The difference in time between this machine and the peer.
    */
    void SyncTime(Pinger* pinger, int32_t time);

    /**
    Called by {@link #SyncSendHandler} to send {@link SyncRequestPacket}s to {@link Pinger}s
    in {@link #unsyncedPingers}.
    */
    void SendSyncRequest(uint8_t destID);

    /**
    Links {@link #syncRequestPack} and {@link #syncReplyPack} callbacks to
    {@link #owner}.
    */
    void LinkCallbacks();

    /**
    Detaches {@link #syncSendThread} to run {@link #SyncSendHandler}.
    */
    void Run();

    /**
    Stops {@link #syncSendThread} from running. Must be called before destructor.
    */
    void Stop();

    /**
    Destruction for SyncManage, deletes syncSendThread and syncReplyPack.
    */
    ~SyncManager();

  private:
    /**
    Prevents {@link #syncSendThread} from running when there are
    no elements in {@link #unsyncedPingers}.
    */
    std::condition_variable syncHandlerCV;
    /**
    Mutex locked by {@link #syncHandlerCV}.
    */
    std::mutex syncHandlerMutex;
    /**
    Prevents multiple threads from accessing {@link #unsyncedPingers}.
    */
    std::mutex unsyncedPingersMutex;
    /**
    Prevents multiple threads from accessing {@link #running}.
    */
    std::mutex runningMutex;
    /**
    Thread that runs {@link #SyncSendHandler}.
    */
    CommThread* syncSendThread;

    /**
    Stores {@link Pingers} that should be send {@link SyncRequestPack}s.
    */
    std::list <Pinger*> unsyncedPingers;

    /**
    Indicates whether unlocking the {@link #syncHandlerCV} was intentional.
    */
    bool awake;

    /**
    True when {@link #syncSendThread} is detached, false otherwise.
    */
    bool running;

    /**
    Packet that will be linked to callback.
    */
    SyncRequestPacket* syncRequestPack;

    /**
    Packet that will be linked to callback.
    */
    SyncReplyPacket* syncReplyPack;

    /**
    The {@link Comms} that owns the {@link PingManager} that owns this.
    */
    Comms* owner;
  };
}
}

#endif
