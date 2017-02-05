#include <CommProto/constate/time_sync_manager.h>
#include <CommProto/constate/time_sync_request.h>
#include <CommProto/constate/time_sync_reply.h>
#include <CommProto/comms.h>

namespace comnet {
namespace constate {
error_t TimeSyncRequestCallback(const comnet::Header & header, TimeSyncRequest & packet, comnet::Comms & node)
{
  TimeSyncReply replyPacket;
  replyPacket.SetRequestSentTime(header.GetSourceTime());
  replyPacket.SetTimeDif(GetUnixTimeMillis(), GetTimeSinceStart());
  node.Send(replyPacket, header.source_id);
  node.GetConStateManager()->CheckResync(header.source_id, packet.GetTimeDif());
  return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
}

error_t TimeSyncReplyCallback(const comnet::Header & header, TimeSyncReply & packet, comnet::Comms & node)
{
  MillisInt estSendTime = (GetTimeSinceStart() - packet.GetRequestSentTime()) / 2;
  MillisInt estDestTimeStamp = header.GetSourceTime() + estSendTime;
  MillisInt timeOff = GetTimeSinceStart() - estDestTimeStamp;
  node.GetConStateManager()->CheckResync(header.source_id, packet.GetTimeDif());
  node.GetConStateManager()->SyncTime(header.source_id, timeOff);
  return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
}
    
TimeSyncManager::TimeSyncManager(Comms * owner)
  :std::enable_shared_from_this <TimeSyncManager>(),
  owner(owner), running(false), syncSendThread(nullptr)
{
  syncRequestPack = new TimeSyncRequest();
  syncReplyPack = new TimeSyncReply();
}

void TimeSyncManager::LinkCallbacks()
{
  owner->LinkCallback(syncRequestPack, new comnet::Callback((comnet::callback_t)TimeSyncRequestCallback));
  owner->LinkCallback(syncReplyPack, new comnet::Callback((comnet::callback_t)TimeSyncReplyCallback));
}

bool TimeSyncManager::Run()
{
  running = true;
  syncSendThread = new CommThread(&TimeSyncManager::SyncSendHandler, shared_from_this());
  syncSendThread->Detach();
  return true;
}

void TimeSyncManager::Stop()
{
  runningMutex.Lock();
  running = false;
  runningMutex.Unlock();
  syncHandlerCV.Set();
}

void TimeSyncManager::AddUnsyncedConState(ConnectionState * conState)
{
  unsyncedConStatesMutex.Lock();
  unsyncedConStates.push_front(conState);
  unsyncedConStatesMutex.Unlock();
  conState->SetInUnsyncedList(true);
  syncHandlerCV.Set();
}

void TimeSyncManager::RemoveUnsyncedConState(ConnectionState * conState)
{
  CommLock lock(unsyncedConStatesMutex);
  unsyncedConStates.remove(conState);
  conState->SetInUnsyncedList(false);
}

void TimeSyncManager::SyncTime(ConnectionState * conState, MillisInt timeOff)
{
  conState->SyncTime(timeOff);
  if (conState->IsSynced())
  {
    RemoveUnsyncedConState(conState);
  }
}

void TimeSyncManager::CheckResync(ConnectionState * conState, UnixMillisInt unixHighResTimeDif)
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

TimeSyncManager::~TimeSyncManager()
{
  free_pointer(syncSendThread);
  free_pointer(syncRequestPack); //may need to remove from comms
  free_pointer(syncReplyPack); //may need to remove from comms
}

void TimeSyncManager::SyncSendHandler()
{
  while (true)
  {
    runningMutex.Lock();
    if (!running) {
      runningMutex.Unlock();
      return;
    }
    unsyncedConStatesMutex.Lock();
    if (!unsyncedConStates.empty())
    {
      std::list <ConnectionState*>::iterator it;
      for (it = unsyncedConStates.begin(); it != unsyncedConStates.end(); it++)
      {
        if ((*it)->GetTimeUntilSendSyncRequest() > 0) {
          break;
        }
        SendSyncRequest((*it)->GetNodeID());
        (*it)->ResetSyncRequestSentTime();
      }
      if (it != unsyncedConStates.end())
      {
        unsyncedConStates.splice(unsyncedConStates.end(), unsyncedConStates, unsyncedConStates.begin(), it);
      }
      MillisInt sleepTime = unsyncedConStates.front()->GetTimeUntilSendSyncRequest();
      runningMutex.Unlock();
      unsyncedConStatesMutex.Unlock();
      syncHandlerCV.Wait((std::chrono::milliseconds)sleepTime);
    }
    else
    {
      unsyncedConStatesMutex.Unlock();
      runningMutex.Unlock();
      syncHandlerCV.Wait();
    }
  }
}

void TimeSyncManager::SendSyncRequest(uint8_t destID)
{
  TimeSyncRequest syncRequest;
  syncRequest.SetTimeDif(GetUnixTimeMillis(), GetTimeSinceStart());
  owner->Send(syncRequest, destID);
}
} //namespace constate
} //namespace comnet
