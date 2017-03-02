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

#include <CommProto/constate/connection_state_manager.h>
#include <CommProto/constate/check_connect_request.h>
#include <CommProto/constate/check_connect_reply.h>
#include <CommProto/constate/time_sync_manager.h>
#include <CommProto/comms.h>

namespace comnet {
namespace constate {

bool ConnectionStateManager::ConStateEnabled = true;

error_t CheckConnectRequestCallback(const comnet::Header & header, CheckConnectRequest & packet, comnet::Comms & node)
{
  if (node.GetConStateManager()->ReadyToSendCheckConnectReply(header.source_id))
  {
    CheckConnectReply checkConReply;
    node.Send(checkConReply, header.source_id);
  }
  return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
}

error_t CheckConnectReplyCallback(const comnet::Header & header, CheckConnectReply & packet, comnet::Comms & node)
{
  return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
}

ConnectionStateManager::ConnectionStateManager(Comms * owner)
  :std::enable_shared_from_this <ConnectionStateManager>(),
  owner(owner), running(false), conStateUpdateThread(nullptr)
{
  checkConRequestPack = new CheckConnectRequest();
  checkConReplyPack = new CheckConnectReply();
  syncManager = std::make_shared<TimeSyncManager>(owner);
}

void ConnectionStateManager::LinkCallbacks()
{
  owner->LinkCallback(checkConRequestPack, new Callback((comnet::callback_t)CheckConnectRequestCallback));
  owner->LinkCallback(checkConReplyPack, new Callback((comnet::callback_t)CheckConnectReplyCallback));
  syncManager->LinkCallbacks();
}

bool ConnectionStateManager::Run()
{
  running = true;
  conStateUpdateThread = new CommThread(&ConnectionStateManager::ConStateUpdateHandler, shared_from_this());
  conStateUpdateThread->Detach();
  syncManager->Run();
  return true;
}

void ConnectionStateManager::Stop()
{
  runningMutex.Lock();
  running = false;
  runningMutex.Unlock();
  conStateHandlerCV.Set();
  syncManager->Stop();
}

void ConnectionStateManager::AddConState(uint8_t nodeID)
{
  activeConStatesMutex.Lock();
  activeConStates.emplace_front(nodeID);
  destConStateMapMutex.Lock();
  destConStateMap.emplace(std::make_pair(nodeID, activeConStates.begin()));
  destConStateMapMutex.Unlock();
  activeConStatesMutex.Unlock();
  conStateHandlerCV.Set();
}

void ConnectionStateManager::RemoveConState(uint8_t nodeID)
{
  CommLock lock(destConStateMapMutex);
  auto mapIter = destConStateMap.find(nodeID);
  if (mapIter != destConStateMap.end())
  {
    if (mapIter->second->IsInUnsyncedList())
    {
      syncManager->RemoveUnsyncedConState(&(*mapIter->second));
    }
    if (!mapIter->second->IsActive()) {
      inactiveConStatesMutex.Lock();
      inactiveConStates.erase(mapIter->second);
      inactiveConStatesMutex.Unlock();
    }
    else
    {
      activeConStatesMutex.Lock();
      activeConStates.erase(mapIter->second);
      activeConStatesMutex.Unlock();
    }
    destConStateMap.erase(mapIter);
  }
}

void ConnectionStateManager::UpdatePing(uint8_t destID, MillisInt time)
{
  destConStateMapMutex.Lock();
  auto mapIter = destConStateMap.find(destID);
  if (mapIter != destConStateMap.end())
  {
    if (!mapIter->second->IsActive())
    {
      activeConStatesMutex.Lock();
      inactiveConStatesMutex.Lock();
      TransferToActiveConStates(mapIter->second);
      inactiveConStatesMutex.Unlock();
      activeConStatesMutex.Unlock();
    }
    else
    {
      activeConStatesMutex.Lock();
      activeConStates.splice(activeConStates.end(), activeConStates, mapIter->second);  //Move ConState to end of the list because its nextPingTime() has been reset to the maximum value.
      activeConStatesMutex.Unlock();
    }
    mapIter->second->ResetCheckConnectRequestSentTimeToSend();
    mapIter->second->UpdatePing(time);
    if (!mapIter->second->IsSynced() && !mapIter->second->IsInUnsyncedList())
    {
      syncManager->AddUnsyncedConState(&*mapIter->second);
    }
  }
  destConStateMapMutex.Unlock();
}

int16_t ConnectionStateManager::GetPing(uint8_t nodeID)
{
  CommLock lock(destConStateMapMutex);
  auto mapIter = destConStateMap.find(nodeID);
  if (mapIter != destConStateMap.end())
  {
    return mapIter->second->GetPing();
  }
  return -1;
}

bool ConnectionStateManager::IsActive(uint8_t nodeID)
{
  CommLock lock(destConStateMapMutex);
  auto mapIt = destConStateMap.find(nodeID);
  if (mapIt != destConStateMap.end())
  {
    return mapIt->second->IsActive();
  }
  return false;
}

bool ConnectionStateManager::IsConnected(uint8_t nodeID)
{
  CommLock lock(destConStateMapMutex);
  auto mapIt = destConStateMap.find(nodeID);
  if (mapIt != destConStateMap.end())
  {
    return mapIt->second->IsConnected();
  }
  return false;
}

void ConnectionStateManager::ResetSendTime(uint8_t nodeID)
{
  destConStateMapMutex.Lock();
  auto mapIter = destConStateMap.find(nodeID);
  if (mapIter != destConStateMap.end()) {
    mapIter->second->ResetCheckConnectReplySentTime();
  }
  destConStateMapMutex.Unlock();
}

bool ConnectionStateManager::ReadyToSendCheckConnectReply(uint8_t nodeID)
{
  CommLock lock(destConStateMapMutex);
  auto mapIter = destConStateMap.find(nodeID);
  if (mapIter != destConStateMap.end())
  {
    if (mapIter->second->GetTimeUntilSendCheckConnectReply() <= 0)
    {
      return true;
    }
  }
  return false;
}

void ConnectionStateManager::SyncTime(uint8_t nodeID, int32_t timeOff)
{
  CommLock lock(destConStateMapMutex);
  auto mapIter = destConStateMap.find(nodeID);
  if (mapIter != destConStateMap.end() && !mapIter->second->IsSynced())
  {
    syncManager->SyncTime(&(*mapIter->second), timeOff);
  }
}

void ConnectionStateManager::CheckResync(uint8_t nodeID, int64_t unixHighResTimeDif)
{
  CommLock lock(destConStateMapMutex);
  auto mapIter = destConStateMap.find(nodeID);
  if (mapIter != destConStateMap.end())
  {
    syncManager->CheckResync(&*mapIter->second, unixHighResTimeDif);
  }
}

ConnectionStateManager::~ConnectionStateManager()
{
  free_pointer(checkConRequestPack);
  free_pointer(checkConReplyPack);
}

void ConnectionStateManager::ConStateUpdateHandler()
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
    //The amount of milliseconds the thread should sleep for after finished
    MillisInt sleepTime;
    activeConStatesMutex.Lock();
    if (!activeConStates.empty())	//While there are ConStates
    {
      //Starts at the ConState with the lowest NextPingTime and ends once no more ConStates need to be sent to (when NextPingTIme is positive)
      //or when the end of the list has been reached.
      auto it = activeConStates.begin();
      while (it != activeConStates.end())
      {
        //Gets the amount of milliseconds until the ConState needs to be sent to
        MillisInt nextPingTime = it->GetTimeUntilSendCheckConnectRequest();
        if (nextPingTime <= 0)
        {
          //When nextPingTime is less than 0 that means its ready to be sent a ping
          SendCheckConReq(it->GetNodeID());
          it->ResetCheckConnectRequestSentTimeToResend();
          if (!it->IsActive()) {
            auto prevIt = it;
            it++;
            inactiveConStatesMutex.Lock();
            TransferToInactiveConStates(prevIt);
            inactiveConStatesMutex.Unlock();
            continue;
          }
        }
        else
        {
          if (it != activeConStates.begin())		//Makes sure that packets actually had their ping time changed
          {
            auto spliceIter = it;	//Iterator representing the position to insert elements into
            while (spliceIter != activeConStates.end() &&
              spliceIter->GetTimeUntilSendCheckConnectRequest() < CHECK_CONNECT_REQUEST_RESEND_DELAY)
            {
              spliceIter++;
            }
            //Puts the ConStates that were sento to into the spliceIter position
            activeConStates.splice(spliceIter, activeConStates, activeConStates.begin(), it);
          }
          break;		//Break because we don't need to send a ping to any other packets
        }
        it++;
      }
    }

    if (activeConStates.empty())
    {
      activeConStatesMutex.Unlock();
      runningMutex.Unlock();
      conStateHandlerCV.Wait();
    }
    else
    {
      sleepTime = activeConStates.front().GetTimeUntilSendCheckConnectRequest();		//The top element should always have the lowest nextPingTime()
      runningMutex.Unlock();
      activeConStatesMutex.Unlock();
      conStateHandlerCV.Wait((std::chrono::milliseconds)sleepTime);
    }
  }
}

void ConnectionStateManager::SendCheckConReq(uint8_t nodeID)
{
  CheckConnectRequest requestPacket;
  owner->Send(requestPacket, nodeID);
}

void ConnectionStateManager::TransferToActiveConStates(std::list<ConnectionState>::iterator it)
{
  activeConStates.splice(activeConStates.end(), inactiveConStates, it);
  conStateHandlerCV.Set();
  std::string debugMsg = "ConState with destID ";
  debugMsg += std::to_string((int)it->GetNodeID());
  debugMsg += " in NodeID ";
  debugMsg += std::to_string((int)owner->GetNodeId());
  debugMsg += " was set to active";
  comnet::debug::Log::Message(comnet::debug::LOG_DEBUG, debugMsg);
}

void ConnectionStateManager::TransferToInactiveConStates(std::list<ConnectionState>::iterator it)
{
  if (it->IsInUnsyncedList()) {
    syncManager->RemoveUnsyncedConState(&(*it));
  }
  inactiveConStates.splice(inactiveConStates.end(), activeConStates, it);
  std::string debugMsg = "ConState with destID ";
  debugMsg += std::to_string((int)it->GetNodeID());
  debugMsg += " in NodeID ";
  debugMsg += std::to_string((int)owner->GetNodeId());
  debugMsg += " was set to inactive";
  comnet::debug::Log::Message(comnet::debug::LOG_DEBUG, debugMsg);
}
} //namespace constate
} //namespace comnet
