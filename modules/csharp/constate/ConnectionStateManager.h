/*
Runs a thread to check the connection of peers. Also runs the TimeSyncManager.

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

#ifndef __CSHARP_CONNECTION_STATE_MANAGER_H
#define __CSHARP_CONNECTION_STATE_MANAGER_H

#include <constate/ConnectionState.h>
#include <CallBack.h>

#include <list>
#include <unordered_map>

#include <cstdint>
#include <vcclr.h>
#using <mscorlib.dll>
#using <System.dll>

namespace Comnet {

ref class Comms;
ref class CommNode;

namespace Constate {
#pragma managed
using namespace System;

ref class TimeSyncManager;
ref class CheckConnectRequest;
ref class CheckConnectReply;

/**
The callback linked for {@link CheckConnectRequest}. Replies with a {@link #CheckConnectReply}
if a packet was not sent recently.
@param header The header data of the packet, only used field is source_id.
@param packet The {@link PingPacket} that will either be a Ping or a Pong.
@param node The {@link Comms} that owns the {@link PingManager} that linked the callback.
@return Tells {@link Comms} how the packet should be finalized.
*/
Int32 CheckConnectRequestCallback(Header^ header, ABSPacket^ packet, CommNode^ node);

/**
The callback linked to {@link CheckConnectReply}. Only specifies how to finalize
the packet.
@param header The header data of the packet, only used field is source_id.
@param packet The {@link PingPacket} that will either be a Ping or a Pong.
@param node The {@link Comms} that owns the {@link PingManager} that linked the callback.
@return Tells {@link Comms} how the packet should be finalized.
*/
Int32 CheckConnectReplyCallback(Header^ header, ABSPacket^ packet, CommNode^ node);
    
typedef gcroot<ConnectionState^> ConnectionState_Ptr;
    
/**
Runs a thread to check the connection of peers. Also runs the {@link TimeSyncManager}.
*/
public ref class ConnectionStateManager
{
public:
  static bool ConStateEnabled = true;

  /**
  Creates a new instance of {@link ConnectionStateManager} setting the {@link #owner} field
  to the argument and initializes the CheckConnect packets.
  @param commsOwner The {@link Comms} that owns this.
  */
  ConnectionStateManager(Comms^ commsOwner);

  /**
  Links {@link CheckConnectRequestCallback} to {@link #checkConRequestPack}
  and {@link CheckConnectReplyCallback} to {@link #checkConReplyCallback}
  in the {@link #owner}.
  */
  void LinkCallbacks();

  /**
  Initializes and detaches the {@link #conStateUpdateThread} to run
  the {@link #ConStateUpdateHandler} method.
  @return True when successfully started, False otherwise.
  */
  bool Run();

  /**
  Must be called before destructor. Stops {@link #conStateUpdateThead}
  and {@link #syncManager} from running.
  */
  void Stop();

  /**
  Creates a new {@link ConnectionState} using the argument and
  adds it to {@link #activePingers}
  and {@link #destPingerMap}.
  @param nodeID The remote node id that the {@link ConnectionState} will be bound to.
  */
  void AddConState(uint8_t nodeID);

  /**
  Removes the {@link ConnectionState} with a nodeID matching the argument.
  @param nodeID The remote node id of the {@link ConnectionState} to remove.
  */
  void RemoveConState(uint8_t nodeID);

  /**
  Calls {@link ConnectionState#ResetCheckConnectRequestSentTimeToSend} on the {@link ConnectionState}
  with the same id as the argument.
  In other words, it moves the corresponding {@link ConnectionState} to the end of {@link #activePingers}.
  @param The node id of the {@link ConnectionState} to reset.
  */
  void UpdatePing(uint8_t destID, MillisInt time);

  /**
  Get the estimated ping in ms of the {@link ConnectionState} corresponding
  to the nodeID.
  @param nodeID The id of the peer to get the ping of.
  @return The ping in milliseconds -1 if invalid.
  */
  int16_t GetPing(uint8_t nodeID);

  /**
  When active packets are being sent to ensure a stable connection
  otherwise the connection is considered broken and attempts to get replys will not longer be made.
  @param nodeID The id of the peer to check for activitiy. Will return false if id was not found.
  @return {@code true} when connection should be checked, {@code false} when the connection is considered broken.
  */
  bool IsActive(uint8_t nodeID);

  /**
  The same as {@link #IsActive} except it will not be true until a packet
  has been received from the peer.
  @param nodeID The id of the peer to check for activitiy. Will return false if id was not found.
  @return {@code true} when connection should be checked, {@code false} when the connection is considered broken.
  */
  bool IsConnected(uint8_t nodeID);

  /**
  Calls {@link ConnectionState#ResetCheckConnectReplySentTime} of the {@link ConnectionState} with the id
  of the argument.
  @param nodeID The id of the {@link ConnectionState} to reset the send time of.
  */
  void ResetSendTime(uint8_t nodeID);

  /**
  Checks if enough time has passed since the last time a packet was sent
  to the remote node_id for a {@link CheckConnectReply} to be sent back
  to a request. This avoids unecessarily sending more packets.
  @param The id of the remote comms node that should be checked.
  @return True if a Pong should be sent, false otherwise.
  */
  bool ReadyToSendCheckConnectReply(uint8_t nodeID);

  /**
  Called by {@link TimeSyncReply} callback to sync timestamp of peer.
  */
  void SyncTime(uint8_t nodeID, MillisInt timeOff);

  /**
  Checks if the node id must be resynced.
  */
  void CheckResync(uint8_t nodeID, UnixMillisInt unixHighResTimeDif);

  /**
    Sets {@link #running} to {@code false} allowing the {@link #conStateUpdateThread} to terminate cleanly.
  */
  !ConnectionStateManager();

  /**
  Calls finalizer.
  */
  ~ConnectionStateManager();

private:
  /**
  The method run by the {@link #conStateUpdateThread}.  Checks if a {@link CheckConnectRequest}
  needs to be sent to any element of {@link #activeConStates}.  Will also deactivate
  {@link ConnectionState}s if enough time has passed without receiving a packet from a client.
  */
  void ConStateUpdateHandler();

  /**
  The thread that runs the {@link #ConStateUpdateHandler}.  Will shut down once
  {@link #running} is set to false.
  */
  Threading::Thread^ conStateUpdateThread;

  /**
  Prevents {@link #ConStateUpdateHandler} from continiousluy running.
  Allowing it to sleep until there is an element in {@link #activeConStates}.
  */
  Threading::AutoResetEvent^ conStateHandlerRE;

  /**
  Indicates whether unlocking the {@link #conStateHandlerCV} was intentional.
  */
  bool awake;

  /**
  {@code true} when the {@link #ConStateUpdateHandler} is running, {@code false} when the thread
  should stop running.
  */
  bool running;

  /**
  Prevents {@link #running} from being modified at the same time {@link #ConStateUpdateHandler}
  is checking it.
  */
  Threading::Mutex^ runningMutex;

  /**
  Sends a {@link CheckConnectRequest} to the specified nodeID.
  @param nodeID The id of the remote node to send the packet to.
  */
  void SendCheckConReq(uint8_t nodeID);

  /**
  Called when a packet was received from a {@link ConnectionState} that was
  considered inactive. The method will move the specified {@link ConnectionState}
  from {@link #inactiveConStates} to {@link #activeConStates}.
  @param it The iterator pointing to the {@link ConnectionState} that should be transfered.
  */
  void TransferToActiveConStates(std::list <ConnectionState_Ptr>::iterator it);

  /**
  Holds all of the active {@link ConnectionState}s. Lowest {@link ConnectionState#GetTimeUntilSendCheckConnectRequest}
  is at the beginning, highest is at the end.
  */
  std::list <ConnectionState_Ptr>* activeConStates;

  /**
  Prevents threads from reading and modifying {@link #activeConStates} at the same time.
  */
  Threading::Mutex^ activeConStatesMutex;

  /**
  Called when {@link ConnectionState} has not responded to requests. Will move
  the {@link ConnectionState} from {@link #activeConStates} to {@link #inactiveConStates}
  so it will no longer be sent anymore {@link CheckConnectRequest}s.
  @param it The iterator pointing to the {@link ConnectionState} that should be transfered.
  */
  void TransferToInactiveConStates(std::list <ConnectionState_Ptr>::iterator it);

  /**
  Holds {@link ConnectionState}s that have been declared as inactive after taking too long to respond.
  */
  std::list <ConnectionState_Ptr>* inactiveConStates;

  /**
  Prevents threads from reading and modifying {@link #inactiveConStates} at the same time.
  */
  Threading::Mutex^ inactiveConStatesMutex;

  /**
  The key is the destination id of the {@link ConnectionState}.  When a packet is received,
  it will use the dest_id to access the corresponding {@link ConnectionState}.
  */
  std::unordered_map <uint8_t, std::list <ConnectionState_Ptr>::iterator>* destConStateMap;

  /**
  Prevents threads from reading and modifying {@link #destConStateMapMutex} at the same time.
  */
  Threading::Mutex^ destConStateMapMutex;

  /**
  Manages the syncing of high resolution time between peers.
  */
  TimeSyncManager^ syncManager;

  /**
  The packet used for linking the callback in {@link #LinkPingCallback}.
  */
  CheckConnectRequest^ checkConRequestPack;

  /**
  The packet used for linking the callback in {@link #LinkPingCallback}.
  */
  CheckConnectReply^ checkConReplyPack;

  /**
  The {@link Comms} object that owns {@code this}.
  */
  Comms^ owner;
};
} // namespace constate
}  // namespace comnet
#endif  //__CSHARP_CONNECTION_STATE_MANAGER_H
