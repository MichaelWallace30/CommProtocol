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

#ifndef __CSHARP_TIME_SYNC_MANAGER_H
#define __CSHARP_TIME_SYNC_MANAGER_H

#include <constate/ConnectionState.h>
#include <CallBack.h>

#include <list>

namespace Comnet
{
ref class Comms;
namespace Constate
{
ref class TimeSyncRequest;
ref class TimeSyncReply;

typedef gcroot<ConnectionState^> ConnectionState_Ptr;

/**
The callback linked to {@link #syncRequestPack}. Will reply with a {@link TimeSyncReply} and
set the unixHighResTimeDif.
@param header The header data of the packet, only used field is source_id.
@param packet The {@link SyncRequestPacket}.
@param node The {@link Comms} that owns the {@link PingManager} that linked the callback.
@return Tells {@link Comms} how the packet should be finalized.
*/
Int32 TimeSyncRequestCallback(Header^ header, ABSPacket^ packet, CommNode^ node);

/**
The callback linked to {@link #syncReplyPack}. Sets the time difference between
the local machine and the peer's high resolution clocks to calculate on way ping.
Also, checks if a resync is necessary.
@param header The header data of the packet, only used field is source_id.
@param packet The {@link SyncRequestPacket}.
@param node The {@link Comms} that owns the {@link PingManager} that linked the callback.
@return Tells {@link Comms} how the packet should be finalized.
*/
Int32 TimeSyncReplyCallback(Header^ header, ABSPacket^ packet, CommNode^ node);

/**
Runs a thread to sync time stamps between this machine and the peer
to calculate one-way ping.
*/
public ref class TimeSyncManager
{
public:
  /**
  Creates a new instance of {@link SyncManager}.
  Initializes {@link #syncRequestPacket} and {@link #syncReplyPacket}.
  @param owner The {@link Comms} that owns this.
  */
  TimeSyncManager(Comms^ owner);

  /**
  Links {@link #syncRequestPack} and {@link #syncReplyPack} callbacks to
  {@link #owner}.
  */
  Void LinkCallbacks();

  /**
  Detaches {@link #syncSendThread} to run {@link #SyncSendHandler}.
  */
  Boolean Run();

  /**
  Stops {@link #syncSendThread} from running. Must be called before destructor.
  */
  Void Stop();

  /**
  Adds a {@link ConnectionState} to {@link #unsyncedConStates} so that the
  {@link #SyncSendHandler} can send messages to it.
  */
  Void AddUnsyncedConState(ConnectionState_Ptr conState);

  /**
  Called when {@link ConnectionState} is inactive, removed, or done syncing.
  @link conState The {@link ConnectionState} to be removed from {@link #unsyncedConStates}.
  */
  Void RemoveUnsyncedConState(ConnectionState_Ptr conState);

  /**
  Called by {@link #TimeSyncReplyCallback}.  Sets the difference in the local machines
  high resolution clock and the peers high resolution clock to calculate ping.
  @param conState The {@link ConnectionState} to adjust the time off on.
  @param time The difference in time between this machine and the peer.
  */
  Void SyncTime(ConnectionState_Ptr conState, MillisInt timeOff);

  /**
  Checks if the conState argument needs to be resynced based on the unixHighResTimeOff.
  If a resync is necessary, this method will call resync on conState and transfer it to
  {@link #unsyncedConStates}.
  @param conState The {@link ConnectionState} to resync.
  @param unixHighResTimeDif The difference between the peer's unix time and high res time.
  */
  Void CheckResync(ConnectionState_Ptr conState, UnixMillisInt unixHighResTimeOff);

  /**
  Deletes syncSendThread and syncReplyPack. {@link #Stop} must be called before
  this destructor.
  */
  !TimeSyncManager();
      
  /**
  Calls the finalizer.
  */
  ~TimeSyncManager();

private:
  /**
  The method run by {@link #syncSendThread}.
  Checks if a {@link #SyncRequestPacket} needs to be sent.
  */
  Void SyncSendHandler();

  /**
  Thread that runs {@link #SyncSendHandler}.
  */
  Threading::Thread^ syncSendThread;

  /**
  Prevents {@link #syncSendThread} from running when there are
  no elements in {@link #unsyncedConStates}.
  */
  Threading::AutoResetEvent^ syncHandlerRE;

  /**
  Called by {@link #SyncSendHandler} to send {@link TimeSyncRequest}s to a {@link ConnectionState}.
  @param destID The node id to send to
  */
  Void SendSyncRequest(uint8_t destID);

  /**
  Stores {@link ConnectionState}s that should be sent {@link TimeSyncRequest}s.
  */
  std::list <ConnectionState_Ptr>* unsyncedConStates;

  /**
  Prevents multiple threads from accessing {@link #unsyncedConStates}.
  */
  Threading::Mutex^ unsyncedConStatesMutex;
  /**
  True when {@link #syncSendThread} is detached, false otherwise.
  */
  bool running;

  /**
  Prevents multiple threads from accessing {@link #running}.
  */
  Threading::Mutex^ runningMutex;

  /**
  Packet that will be linked to callback.
  */
  TimeSyncRequest^ syncRequestPack;

  /**
  Packet that will be linked to callback.
  */
  TimeSyncReply^ syncReplyPack;

  /**
  The {@link Comms} that owns the {@link ConnectionStateManager} that owns this.
  */
  Comms^ owner;
};
} //namespace constate
} //namespace comnet

#endif //__CSHARP_TIME_SYNC_MANAGER_H
