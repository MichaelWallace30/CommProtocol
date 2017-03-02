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

#include <constate/ConnectionStateManager.h>
#include <constate/CheckConnectRequest.h>
#include <constate/CheckConnectReply.h>
#include <constate/TimeSyncManager.h>
#include <Header.h>
#include <Comms.h>

namespace Comnet {
namespace Constate {

Int32 CheckConnectRequestCallback(Header^ header, ABSPacket^ packet, CommNode^ node)
{
  Comms^ comms = (Comms^)node;
      
  if (comms->GetConStateManager()->ReadyToSendCheckConnectReply(header->GetSourceID()))
  {
    CheckConnectReply^ checkConReply = gcnew CheckConnectReply();
    comms->Send(checkConReply, header->GetSourceID());
  }
  return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
}

Int32 CheckConnectReplyCallback(Header^ header, ABSPacket^ packet, CommNode^ node)
{
  return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
}

ConnectionStateManager::ConnectionStateManager(Comms^ owner)
  :owner(owner), running(false), awake(false), conStateUpdateThread(nullptr)
{
  checkConRequestPack = gcnew CheckConnectRequest();
  checkConReplyPack = gcnew CheckConnectReply();
  syncManager = gcnew TimeSyncManager(owner);
  activeConStates = new std::list<ConnectionState_Ptr>();
  inactiveConStates = new std::list<ConnectionState_Ptr>();
  destConStateMap = new std::unordered_map <uint8_t, std::list <ConnectionState_Ptr>::iterator>();
      
  runningMutex = gcnew Threading::Mutex();
  activeConStatesMutex = gcnew Threading::Mutex();
  inactiveConStatesMutex = gcnew Threading::Mutex();
  destConStateMapMutex = gcnew Threading::Mutex();
  conStateHandlerRE = gcnew Threading::AutoResetEvent(false);
}

void ConnectionStateManager::LinkCallbacks()
{
  owner->LinkCallback(checkConRequestPack, gcnew CallBack(gcnew CommFunct(CheckConnectRequestCallback)));
  owner->LinkCallback(checkConReplyPack, gcnew CallBack(gcnew CommFunct(CheckConnectReplyCallback)));
  syncManager->LinkCallbacks();
}

bool ConnectionStateManager::Run()
{
  running = true;
  conStateUpdateThread = gcnew Threading::Thread(gcnew Threading::ThreadStart(this, &ConnectionStateManager::ConStateUpdateHandler));
  conStateUpdateThread->Start();
  syncManager->Run();
  return true;
}

void ConnectionStateManager::Stop()
{
  runningMutex->WaitOne();
  running = false;
  runningMutex->ReleaseMutex();
  conStateHandlerRE->Set();
  syncManager->Stop();
}

void ConnectionStateManager::AddConState(uint8_t nodeID)
{
  activeConStatesMutex->WaitOne();
  activeConStates->push_front(gcroot<ConnectionState^>(gcnew ConnectionState(nodeID)));
  destConStateMapMutex->WaitOne();
  destConStateMap->emplace(std::make_pair(nodeID, activeConStates->begin()));
  destConStateMapMutex->ReleaseMutex();
  activeConStatesMutex->ReleaseMutex();
  conStateHandlerRE->Set();
}

void ConnectionStateManager::RemoveConState(uint8_t nodeID)
{
  destConStateMapMutex->WaitOne();
  auto mapIter = destConStateMap->find(nodeID);
  if (mapIter != destConStateMap->end())
  {
    if ((*mapIter->second)->IsInUnsyncedList())
    {
      syncManager->RemoveUnsyncedConState(*mapIter->second);
    }
    if (!(*mapIter->second)->IsActive()) {
      inactiveConStatesMutex->WaitOne();
      inactiveConStates->erase(mapIter->second);
      inactiveConStatesMutex->ReleaseMutex();
    }
    else
    {
      activeConStatesMutex->WaitOne();
      activeConStates->erase(mapIter->second);
      activeConStatesMutex->ReleaseMutex();
    }
    destConStateMap->erase(mapIter);
  }
  destConStateMapMutex->ReleaseMutex();
}

void ConnectionStateManager::UpdatePing(uint8_t destID, MillisInt time)
{
  destConStateMapMutex->WaitOne();
  auto mapIter = destConStateMap->find(destID);
  if (mapIter != destConStateMap->end())
  {
    if (!(*mapIter->second)->IsActive())
    {
      activeConStatesMutex->WaitOne();
      inactiveConStatesMutex->WaitOne();
      TransferToActiveConStates(mapIter->second);
      inactiveConStatesMutex->ReleaseMutex();
      activeConStatesMutex->ReleaseMutex();
    }
    else
    {
      activeConStatesMutex->WaitOne();
      activeConStates->splice(activeConStates->end(), *activeConStates, mapIter->second);  //Move ConState to end of the list because its nextPingTime() has been reset to the maximum value.
      activeConStatesMutex->ReleaseMutex();
    }
    (*mapIter->second)->ResetCheckConnectRequestSentTimeToSend();
    (*mapIter->second)->UpdatePing(time);
    if (!(*mapIter->second)->IsSynced() && !(*mapIter->second)->IsInUnsyncedList())
    {
      syncManager->AddUnsyncedConState(*mapIter->second);
    }
  }
  destConStateMapMutex->ReleaseMutex();
}

int16_t ConnectionStateManager::GetPing(uint8_t nodeID)
{
  PingMillisInt ping = -1;
  destConStateMapMutex->WaitOne();
  auto mapIter = destConStateMap->find(nodeID);
  if (mapIter != destConStateMap->end())
  {
    ping = (*mapIter->second)->GetPing();
  }
  destConStateMapMutex->ReleaseMutex();
  return ping;
}

bool ConnectionStateManager::IsActive(uint8_t nodeID)
{
  bool isActive = false;
  destConStateMapMutex->WaitOne();
  auto mapIt = destConStateMap->find(nodeID);
  if (mapIt != destConStateMap->end())
  {
    isActive = (*mapIt->second)->IsActive();
  }
  destConStateMapMutex->ReleaseMutex();
  return isActive;
}

bool ConnectionStateManager::IsConnected(uint8_t nodeID)
{
  bool isConnected = false;
  destConStateMapMutex->WaitOne();
  auto mapIt = destConStateMap->find(nodeID);
  if (mapIt != destConStateMap->end())
  {
    return (*mapIt->second)->IsConnected();
  }
  destConStateMapMutex->ReleaseMutex();
  return isConnected;
}

void ConnectionStateManager::ResetSendTime(uint8_t nodeID)
{
  destConStateMapMutex->WaitOne();
  auto mapIter = destConStateMap->find(nodeID);
  if (mapIter != destConStateMap->end()) {
    (*mapIter->second)->ResetCheckConnectReplySentTime();
  }
  destConStateMapMutex->ReleaseMutex();
}

bool ConnectionStateManager::ReadyToSendCheckConnectReply(uint8_t nodeID)
{
  bool ready = false;
  destConStateMapMutex->WaitOne();
  auto mapIter = destConStateMap->find(nodeID);
  if (mapIter != destConStateMap->end())
  {
    if ((*mapIter->second)->GetTimeUntilSendCheckConnectReply() <= 0)
    {
      ready = true;
    }
  }
  destConStateMapMutex->ReleaseMutex();
  return ready;
}

void ConnectionStateManager::SyncTime(uint8_t nodeID, MillisInt timeOff)
{
  destConStateMapMutex->WaitOne();
  auto mapIter = destConStateMap->find(nodeID);
  if (mapIter != destConStateMap->end() && !(*mapIter->second)->IsSynced())
  {
    syncManager->SyncTime(*mapIter->second, timeOff);
  }
  destConStateMapMutex->ReleaseMutex();
}

void ConnectionStateManager::CheckResync(uint8_t nodeID, UnixMillisInt unixHighResTimeDif)
{
  destConStateMapMutex->WaitOne();
  auto mapIter = destConStateMap->find(nodeID);
  if (mapIter != destConStateMap->end())
  {
    syncManager->CheckResync(*mapIter->second, unixHighResTimeDif);
  }
  destConStateMapMutex->ReleaseMutex();
}

ConnectionStateManager::!ConnectionStateManager()
{
  delete activeConStates;
  activeConStates = nullptr;
  delete inactiveConStates;
  inactiveConStates = nullptr;
  delete destConStateMap;
  destConStateMap = nullptr;
}

ConnectionStateManager::~ConnectionStateManager()
{
  this->!ConnectionStateManager();
}

void ConnectionStateManager::ConStateUpdateHandler()
{
  while (true)
  {
    runningMutex->WaitOne();
    //Checks if the thread should still be running
    if (!running)
    {
      runningMutex->ReleaseMutex();
      return;
    }
    //The amount of milliseconds the thread should sleep for after finished
    activeConStatesMutex->WaitOne();
    if (!activeConStates->empty())	//While there are ConStates
    {
      //Starts at the ConState with the lowest NextPingTime and ends once no more ConStates need to be sent to (when NextPingTIme is positive)
      //or when the end of the list has been reached.
      auto it = activeConStates->begin();
      while (it != activeConStates->end())
      {
        //Gets the amount of milliseconds until the ConState needs to be sent to
        MillisInt nextPingTime = (*it)->GetTimeUntilSendCheckConnectRequest();
        if (nextPingTime <= 0)
        {
          //When nextPingTime is less than 0 that means its ready to be sent a ping
          SendCheckConReq((*it)->GetNodeID());
          (*it)->ResetCheckConnectRequestSentTimeToResend();
          if (!(*it)->IsActive()) {
            auto prevIt = it;
            it++;
            inactiveConStatesMutex->WaitOne();
            TransferToInactiveConStates(prevIt);
            inactiveConStatesMutex->ReleaseMutex();
            continue;
          }
        }
        else
        {
          if (it != activeConStates->begin())		//Makes sure that packets actually had their ping time changed
          {
            auto spliceIter = it;	//Iterator representing the position to insert elements into
            while (spliceIter != activeConStates->end() &&
              (*spliceIter)->GetTimeUntilSendCheckConnectRequest() < CHECK_CONNECT_REQUEST_RESEND_DELAY)
            {
              spliceIter++;
            }
            //Puts the ConStates that were sento to into the spliceIter position
            activeConStates->splice(spliceIter, *activeConStates, activeConStates->begin(), it);
          }
          break;		//Break because we don't need to send a ping to any other packets
        }
        it++;
      }
    }

    runningMutex->ReleaseMutex();
    if (activeConStates->empty())
    {
      activeConStatesMutex->ReleaseMutex();
      conStateHandlerRE->WaitOne();
    }
    else
    {
      activeConStatesMutex->ReleaseMutex();
      MillisInt sleepTime = activeConStates->front()->GetTimeUntilSendCheckConnectRequest();
      if (sleepTime < 0)
      {
        sleepTime = 0;
      }
      conStateHandlerRE->WaitOne(sleepTime);
    }
  }
}

void ConnectionStateManager::SendCheckConReq(uint8_t nodeID)
{
  CheckConnectRequest^ requestPacket = gcnew CheckConnectRequest();
  owner->Send(requestPacket, nodeID);
}

void ConnectionStateManager::TransferToActiveConStates(std::list<ConnectionState_Ptr>::iterator it)
{
  activeConStates->splice(activeConStates->end(), *inactiveConStates, it);
  conStateHandlerRE->Set();
  std::string debugMsg = "ConState with destID ";
  debugMsg += std::to_string((int)(*it)->GetNodeID());
  debugMsg += " in NodeID ";
  debugMsg += std::to_string((int)owner->GetNodeId());
  debugMsg += " was set to active";
  comnet::debug::Log::Message(comnet::debug::LOG_DEBUG, debugMsg);
}

void ConnectionStateManager::TransferToInactiveConStates(std::list<ConnectionState_Ptr>::iterator it)
{
  if ((*it)->IsInUnsyncedList()) {
    syncManager->RemoveUnsyncedConState(*it);
  }
  inactiveConStates->splice(inactiveConStates->end(), *activeConStates, it);
  std::string debugMsg = "ConState with destID ";
  debugMsg += std::to_string((int)(*it)->GetNodeID());
  debugMsg += " in NodeID ";
  debugMsg += std::to_string((int)owner->GetNodeId());
  debugMsg += " was set to inactive";
  comnet::debug::Log::Message(comnet::debug::LOG_DEBUG, debugMsg);
}
} //namespace constate
} //namespace comnet
