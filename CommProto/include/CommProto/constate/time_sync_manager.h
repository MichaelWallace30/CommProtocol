/*
Runs a thread to sync time stamps between this machine and the peer
to calculate one-way ping.

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

#ifndef __TIME_SYNC_MANAGER_H
#define __TIME_SYNC_MANAGER_H

#include <CommProto/constate/connection_state.h>

#include <CommProto/callback.h>
#include <CommProto/architecture/os/comm_thread.h>
#include <CommProto/architecture/os/comm_mutex.h>
#include <CommProto/architecture/os/comm_condvar.h>

#include <mutex>
#include <list>
#include <memory>

namespace comnet
{
class Comms;
namespace constate
{
using namespace comnet::architecture::os;

class TimeSyncRequest;
class TimeSyncReply;

/**
The callback linked to {@link #syncRequestPack}. Will reply with a {@link TimeSyncReply} and
set the unixHighResTimeDif.
@param header The header data of the packet, only used field is source_id.
@param packet The {@link SyncRequestPacket}.
@param node The {@link Comms} that owns the {@link PingManager} that linked the callback.
@return Tells {@link Comms} how the packet should be finalized.
*/
error_t TimeSyncRequestCallback(const comnet::Header& header, TimeSyncRequest& packet, comnet::Comms& node);

/**
The callback linked to {@link #syncReplyPack}. Sets the time difference between
the local machine and the peer's high resolution clocks to calculate on way ping.
Also, checks if a resync is necessary.
@param header The header data of the packet, only used field is source_id.
@param packet The {@link SyncRequestPacket}.
@param node The {@link Comms} that owns the {@link PingManager} that linked the callback.
@return Tells {@link Comms} how the packet should be finalized.
*/
error_t TimeSyncReplyCallback(const comnet::Header& header, TimeSyncReply& packet, comnet::Comms& node);

/**
Runs a thread to sync time stamps between this machine and the peer
to calculate one-way ping.
*/
class TimeSyncManager : public std::enable_shared_from_this<TimeSyncManager>
{
public:
  /**
  Creates a new instance of {@link SyncManager}.
  Initializes {@link #syncRequestPacket} and {@link #syncReplyPacket}.
  @param owner The {@link Comms} that owns this.
  */
  TimeSyncManager(Comms* owner);

  /**
  Links {@link #syncRequestPack} and {@link #syncReplyPack} callbacks to
  {@link #owner}.
  */
  void LinkCallbacks();

  /**
  Detaches {@link #syncSendThread} to run {@link #SyncSendHandler}.
  */
  bool Run();

  /**
  Stops {@link #syncSendThread} from running. Must be called before destructor.
  */
  void Stop();

  /**
  Adds a {@link ConnectionState} to {@link #unsyncedConStates} so that the
  {@link #SyncSendHandler} can send messages to it.
  */
  void AddUnsyncedConState(ConnectionState* conState);

  /**
  Called when {@link ConnectionState} is inactive, removed, or done syncing.
  @link conState The {@link ConnectionState} to be removed from {@link #unsyncedConStates}.
  */
  void RemoveUnsyncedConState(ConnectionState* conState);

  /**
  Called by {@link #TimeSyncReplyCallback}.  Sets the difference in the local machines
  high resolution clock and the peers high resolution clock to calculate ping.
  @param conState The {@link ConnectionState} to adjust the time off on.
  @param time The difference in time between this machine and the peer.
  */
  void SyncTime(ConnectionState* conState, MillisInt timeOff);

  /**
  Checks if the conState argument needs to be resynced based on the unixHighResTimeOff.
  If a resync is necessary, this method will call resync on conState and transfer it to
  {@link #unsyncedConStates}.
  @param conState The {@link ConnectionState} to resync.
  @param unixHighResTimeDif The difference between the peer's unix time and high res time.
  */
  void CheckResync(ConnectionState* conState, UnixMillisInt unixHighResTimeOff);

  /**
  Deletes syncSendThread and syncReplyPack. {@link #Stop} must be called before
  this destructor.
  */
  ~TimeSyncManager();

private:
  /**
  The method run by {@link #syncSendThread}.
  Checks if a {@link #SyncRequestPacket} needs to be sent.
  */
  void SyncSendHandler();

  /**
  Thread that runs {@link #SyncSendHandler}.
  */
  CommThread* syncSendThread;

  /**
  Prevents {@link #syncSendThread} from running when there are
  no elements in {@link #unsyncedConStates}.
  */
  CommConditionVariable syncHandlerCV;

  /**
  Called by {@link #SyncSendHandler} to send {@link TimeSyncRequest}s to a {@link ConnectionState}.
  @param destID The node id to send to
  */
  void SendSyncRequest(uint8_t destID);

  /**
  Stores {@link ConnectionState}s that should be sent {@link TimeSyncRequest}s.
  */
  std::list <ConnectionState*> unsyncedConStates;

  /**
  Prevents multiple threads from accessing {@link #unsyncedConStates}.
  */
  CommMutex unsyncedConStatesMutex;
  /**
  True when {@link #syncSendThread} is detached, false otherwise.
  */
  bool running;

  /**
  Prevents multiple threads from accessing {@link #running}.
  */
  CommMutex runningMutex;

  /**
  Packet that will be linked to callback.
  */
  TimeSyncRequest* syncRequestPack;

  /**
  Packet that will be linked to callback.
  */
  TimeSyncReply* syncReplyPack;

  /**
  The {@link Comms} that owns the {@link ConnectionStateManager} that owns this.
  */
  Comms* owner;
};
} //namespace constate
} //namespace comnet

#endif
