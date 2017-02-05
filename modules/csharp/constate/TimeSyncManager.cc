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

#include <constate/TimeSyncManager.h>
#include <constate/TimeSyncRequest.h>
#include <constate/TimeSyncReply.h>
#include <Comms.h>

namespace Comnet {
namespace Constate {
Int32 TimeSyncRequestCallback(Header^ header, ABSPacket^ packet, CommNode^ node)
{
  TimeSyncReply^ replyPacket = gcnew TimeSyncReply();
  replyPacket->SetRequestSentTime(header->GetSourceTime());
  replyPacket->SetTimeDif(GetUnixTimeMillis(), GetTimeSinceStart());
  ((Comms^)node)->Send(replyPacket, header->GetSourceID());
  ((Comms^)node)->GetConStateManager()->CheckResync(header->GetSourceID(), ((TimeSyncRequest^)packet)->GetTimeDif());
  return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
}

Int32 TimeSyncReplyCallback(Header^ header, ABSPacket^ packet, CommNode^ node)
{
  TimeSyncReply^ timeSyncReply = (TimeSyncReply^)packet;
  MillisInt estSendTime = (GetTimeSinceStart() - timeSyncReply->GetRequestSentTime()) / 2;
  MillisInt estDestTimeStamp = header->GetSourceTime() + estSendTime;
  MillisInt timeOff = GetTimeSinceStart() - estDestTimeStamp;
  ((Comms^)node)->GetConStateManager()->CheckResync(header->GetSourceID(), timeSyncReply->GetTimeDif());
  ((Comms^)node)->GetConStateManager()->SyncTime(header->GetSourceID(), timeOff);
  return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
}

TimeSyncManager::TimeSyncManager(Comms^ owner)
  :owner(owner), running(false), syncSendThread(nullptr)
{
  syncRequestPack = gcnew TimeSyncRequest();
  syncReplyPack = gcnew TimeSyncReply();
  unsyncedConStates = new std::list<ConnectionState_Ptr>();
  syncHandlerRE = gcnew Threading::AutoResetEvent(false);
  runningMutex = gcnew Threading::Mutex();
  unsyncedConStatesMutex = gcnew Threading::Mutex();
}

void TimeSyncManager::LinkCallbacks()
{
  owner->LinkCallback(syncRequestPack, gcnew CallBack(gcnew CommFunct(Comnet::Constate::TimeSyncRequestCallback)));
  owner->LinkCallback(syncReplyPack, gcnew CallBack(gcnew CommFunct(Comnet::Constate::TimeSyncReplyCallback)));
}

bool TimeSyncManager::Run()
{
  running = true;
  syncSendThread = gcnew Threading::Thread(gcnew Threading::ThreadStart(this, &TimeSyncManager::SyncSendHandler));
  syncSendThread->Start();
  return true;
}

void TimeSyncManager::Stop()
{
  runningMutex->WaitOne();
  running = false;
  runningMutex->ReleaseMutex();
  syncHandlerRE->Set();
}

void TimeSyncManager::AddUnsyncedConState(ConnectionState_Ptr conState)
{
  unsyncedConStatesMutex->WaitOne();
  unsyncedConStates->push_front(conState);
  unsyncedConStatesMutex->ReleaseMutex();
  syncHandlerRE->Set();
  conState->SetInUnsyncedList(true);
}

void TimeSyncManager::RemoveUnsyncedConState(ConnectionState_Ptr conState)
{
  unsyncedConStatesMutex->WaitOne();
  for (auto it = unsyncedConStates->begin(); it != unsyncedConStates->end(); it++)
  {
    if ((*it)->GetNodeID() == conState->GetNodeID())
    {
      unsyncedConStates->erase(it);
      conState->SetInUnsyncedList(false);
      break;
    }
  }
  unsyncedConStatesMutex->ReleaseMutex();
}

void TimeSyncManager::SyncTime(ConnectionState_Ptr conState, MillisInt timeOff)
{
  conState->SyncTime(timeOff);
  if (conState->IsSynced())
  {
    RemoveUnsyncedConState(conState);
  }
}

void TimeSyncManager::CheckResync(ConnectionState_Ptr conState, UnixMillisInt unixHighResTimeDif)
{
  if (conState->IsResyncRequired(unixHighResTimeDif))
  {
    if (conState->IsSynced())
    {
      AddUnsyncedConState(conState);
    }
    conState->Resync();
  }
  conState->SetUnixHighResTimeDif(unixHighResTimeDif);
}

TimeSyncManager::!TimeSyncManager()
{
  delete unsyncedConStates;
  unsyncedConStates = nullptr;
}

TimeSyncManager::~TimeSyncManager()
{
  this->!TimeSyncManager();
}

void TimeSyncManager::SyncSendHandler()
{
  while (true)
  {
    runningMutex->WaitOne();
    if (!running) {
      runningMutex->ReleaseMutex();
      return;
    }
    unsyncedConStatesMutex->WaitOne();
    if (!unsyncedConStates->empty())
    {
      std::list <ConnectionState_Ptr>::iterator it;
      for (it = unsyncedConStates->begin(); it != unsyncedConStates->end(); it++)
      {
        if ((*it)->GetTimeUntilSendSyncRequest() > 0) {
          break;
        }
        SendSyncRequest((*it)->GetNodeID());
        (*it)->ResetSyncRequestSentTime();
      }
      if (it != unsyncedConStates->end())
      {
        unsyncedConStates->splice(unsyncedConStates->end(), *unsyncedConStates, unsyncedConStates->begin(), it);
      }
      runningMutex->ReleaseMutex();
      MillisInt sleepTime = unsyncedConStates->front()->GetTimeUntilSendSyncRequest();
      unsyncedConStatesMutex->ReleaseMutex();
      if (sleepTime < 0)
      {
        sleepTime = 0;
      }
      syncHandlerRE->WaitOne(sleepTime);
    }
    else
    {
      runningMutex->ReleaseMutex();
      unsyncedConStatesMutex->ReleaseMutex();
      syncHandlerRE->WaitOne();
    }
  }
}

void TimeSyncManager::SendSyncRequest(uint8_t destID)
{
  TimeSyncRequest^ syncRequest = gcnew TimeSyncRequest();
  syncRequest->SetTimeDif(GetUnixTimeMillis(), GetTimeSinceStart());
  owner->Send(syncRequest, destID);
}
} //namespace constate
} //namespace comnet
