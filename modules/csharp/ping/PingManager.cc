#include <ping/PingManager.h>
#include <ping/PingPacket.h>
#include <ping/SyncManager.h>
#include <Comms.h>
#include <iostream>

namespace Comnet {
  namespace Ping {
    Int32 PingCallback(Header^ header, ABSPacket^ absPacket, CommNode^ node) {
      PingPacket^ packet = static_cast<PingPacket^>(absPacket);
      Comms^ comms = static_cast<Comms^>(node);
      
      if (packet->IsPing())
      {
        if (comms->GetPingManager()->CanPong(header->GetSourceID()))
        {
          PingPacket^ pingPacket = gcnew PingPacket();
          pingPacket->SetPing(false);
          comms->Send(pingPacket, header->GetSourceID());
        }
      }
      return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
    }

    PingManager::PingManager(Comms^ owner)
      :owner(owner), pingSendThread(nullptr)
    {
      pingPacket = gcnew PingPacket();
      activePingers = new std::list<Pinger_Ptr>();
      inactivePingers = new std::list<Pinger_Ptr>();
      destPingerMap = new std::unordered_map<uint8_t, std::list<Pinger_Ptr>::iterator>();

      activePingersMutex = gcnew Threading::Mutex();
      inactivePingersMutex = gcnew Threading::Mutex();
      destPingerMapMutex = gcnew Threading::Mutex();
      runningMutex = gcnew Threading::Mutex();

						syncManager = gcnew SyncManager(owner);
    }

    Void PingManager::LinkPingCallback()
    {
      owner->LinkCallback(pingPacket, gcnew CallBack(gcnew CommFunct(Comnet::Ping::PingCallback)));
						syncManager->LinkCallbacks();
				}

    Boolean PingManager::Run()
    {
      running = true;
      pingSendThread = gcnew Threading::Thread(gcnew Threading::ThreadStart(this, &PingManager::HandlePingUpdate));
      pingSendThread->Start();
						syncManager->Run();
      return true;
    }

    void PingManager::Stop()
    {
      runningMutex->WaitOne();
      running = false;
      runningMutex->ReleaseMutex();
						syncManager->Stop();
    }
    
    Boolean PingManager::IsActive(uint8_t destID)
    {
      bool result = false;
      destPingerMapMutex->WaitOne();
      auto mapIt = destPingerMap->find(destID);
      if (mapIt != destPingerMap->end())
      {
        result = !(*mapIt->second)->IsInactive();
      }
      destPingerMapMutex->ReleaseMutex();
      return result;
    }

    Void PingManager::ResetPingTime(uint8_t destID, int32_t time)
    {
      destPingerMapMutex->WaitOne();
      auto mapIter = destPingerMap->find(destID);
      if (mapIter != destPingerMap->end())
      {
        if ((*mapIter->second)->IsInactive())
        {
          activePingersMutex->WaitOne();
          inactivePingersMutex->WaitOne();
          TransferToActivePingers(mapIter->second);
          inactivePingersMutex->ReleaseMutex();
          activePingersMutex->ReleaseMutex();
        }
        else
        {
          activePingersMutex->WaitOne();
          activePingers->splice(activePingers->end(), *activePingers, mapIter->second);
          activePingersMutex->ReleaseMutex();
        }
        (*mapIter->second)->ResetReceiveTime();
								(*mapIter->second)->ResetPing(time);
      }
      destPingerMapMutex->ReleaseMutex();
    }

    Void PingManager::ResetSendTime(uint8_t destID)
    {
      destPingerMapMutex->WaitOne();
      auto mapIter = destPingerMap->find(destID);
      if (mapIter != destPingerMap->end())
      {
        (*mapIter->second)->ResetSendTime();
      }
      destPingerMapMutex->ReleaseMutex();
    }

    Void PingManager::HandlePingUpdate()
    {
      while (true)
      {
        runningMutex->WaitOne();
        if (!running)
        {
          runningMutex->ReleaseMutex();
          return;
        }
        runningMutex->ReleaseMutex();
        MillisInt sleepTime;
        activePingersMutex->WaitOne();
        if (!activePingers->empty()) {
          //Starts at the pinger with the lowest NextPingTime and ends once no more pingers need to be sent to (when NextPingTIme is positive)
          //or when the end of the list has been reached.
          auto it = activePingers->begin();
          while (it != activePingers->end())
          {
            //Gets the amount of milliseconds until the pinger needs to be sent to
            MillisInt nextPingTime = (*it)->GetNextPingTimeMillis();
            //When nextPingTime is less than 0 that means its ready to be sent a ping
            if (nextPingTime <= 0)
            {
              SendPingPacket((*it)->GetDestID());		//Sends a ping packet to the pinger
              (*it)->ResetToResendPingTime();		//Will reset nextPingTime so that it will only be negative after Pinger::PING_RESEND_TIME_MILLIS has passed
              if ((*it)->IsInactive()) {
                auto prevIt = it;
                it++;
                inactivePingersMutex->WaitOne();
                TransferToInactivePingers(prevIt);
                inactivePingersMutex->ReleaseMutex();
                continue;
              }
            }
            else
            {
              if (it != activePingers->begin())		//Makes sure that packets actually had their ping time changed
              {
                auto spliceIter = it;	//Iterator representing the position to insert elements into
                while (spliceIter != activePingers->end() && (*spliceIter)->GetNextPingTimeMillis() < Pinger::PING_RESEND_TIME_MILLIS)
                {
                  spliceIter++;
                }
                //Puts the pingers that were sento to into the spliceIter position
                activePingers->splice(spliceIter, *activePingers, activePingers->begin(), it);
              }
              break;		//Break because we don't need to send a ping to any other packets
            }
            it++;
          }
        }
        if (activePingers->empty())
        {
          sleepTime = EMPTY_SLEEP_TIME_MILLIS;
        }
        else
        {
          sleepTime = activePingers->front()->GetNextPingTimeMillis();
          if (sleepTime < 0)
          {
            sleepTime = 1;  //prevents C# out of bounds error when provided with a negative sleepTime
          }
        }
        activePingersMutex->ReleaseMutex();
        System::Threading::Thread::Sleep(sleepTime);
      }
    }

    Void PingManager::AddPinger(uint8_t destID)
    {
      activePingersMutex->WaitOne();
      activePingers->push_back(gcroot<Pinger^>(gcnew Pinger(destID)));
      destPingerMapMutex->WaitOne();
      destPingerMap->emplace(std::make_pair(destID, --activePingers->end()));
      destPingerMapMutex->ReleaseMutex();
						syncManager->AddUnsyncedPinger(activePingers->back());
      activePingersMutex->ReleaseMutex();
    }

    Void PingManager::RemovePinger(uint8_t destID)
    {
      destPingerMapMutex->WaitOne();
      auto mapIter = destPingerMap->find(destID);
      if (mapIter != destPingerMap->end())
      {
								if (!(*mapIter->second)->IsSynced())
								{
										syncManager->RemoveUnsyncedPinger(*mapIter->second);
								}
        if ((*mapIter->second)->IsInactive())
        {
          inactivePingersMutex->WaitOne();
          inactivePingers->erase(mapIter->second);
          inactivePingersMutex->ReleaseMutex();
        }
        else
        {
          activePingersMutex->WaitOne();
          activePingers->erase(mapIter->second);
          activePingersMutex->ReleaseMutex();
        }
        destPingerMap->erase(mapIter);
      }
      destPingerMapMutex->ReleaseMutex();
    }

    Boolean PingManager::CanPong(uint8_t destID)
    {
      bool result = false;
      destPingerMapMutex->WaitOne();
      auto mapIter = destPingerMap->find(destID);
      if (mapIter != destPingerMap->end())
      {
        if ((*mapIter->second)->GetSendTimePassed() > Pinger::PONG_TIME_MILLIS)
        {
          result = true;
        }
      }
      destPingerMapMutex->ReleaseMutex();
      return result;
    }

    Void PingManager::SendPingPacket(uint8_t destID) {
      PingPacket^ sendPacket = gcnew PingPacket();
      sendPacket->SetPing(true);
      owner->Send(sendPacket, destID);
    }

				Void PingManager::SyncTime(uint8_t nodeID, int32_t timeOff)
				{
						destPingerMapMutex->WaitOne();
						auto mapIter = destPingerMap->find(nodeID);
						if (mapIter != destPingerMap->end() && !(*mapIter->second)->IsSynced())
						{
								syncManager->SyncTime(*mapIter->second, timeOff);
						}
						destPingerMapMutex->ReleaseMutex();
				}

				Void PingManager::CheckResync(uint8_t nodeID, int64_t unixHighResTimeDif)
				{
						destPingerMapMutex->WaitOne();
						auto mapIter = destPingerMap->find(nodeID);
						if (mapIter != destPingerMap->end())
						{
								std::cout << "UNIX TIME DIF: " << unixHighResTimeDif << std::endl;
								if ((*mapIter->second)->CheckResync(unixHighResTimeDif))
								{
										if ((*mapIter->second)->IsSynced())
										{
												syncManager->AddUnsyncedPinger(*mapIter->second);
										}
										(*mapIter->second)->Resync();
										std::cout << "RESYNC CALLED\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
								}
								(*mapIter->second)->SetUnixHighResTimeDif(unixHighResTimeDif);
						}
						destPingerMapMutex->ReleaseMutex();
				}

				int16_t PingManager::GetPing(uint8_t nodeID)
				{
						int16_t ping;
						destPingerMapMutex->WaitOne();
						auto mapIter = destPingerMap->find(nodeID);
						if (mapIter != destPingerMap->end())
						{
								ping = (*mapIter->second)->GetPing();
						}
						destPingerMapMutex->ReleaseMutex();
						return ping;
				}

    PingManager::~PingManager()
    {
      this->!PingManager();
    }

    PingManager::!PingManager()
    {
      delete activePingers;
      activePingers = nullptr;
      delete inactivePingers;
      inactivePingers = nullptr;
      delete destPingerMap;
      destPingerMap = nullptr;
    }

    Void PingManager::TransferToActivePingers(std::list <Pinger_Ptr>::iterator it)
    {
						if (!(*it)->IsSynced())
						{
								syncManager->AddUnsyncedPinger(*it);
						}
      activePingers->splice(activePingers->end(), *inactivePingers, it);
      std::string debugMsg = "Pinger with destID ";
      debugMsg += std::to_string((int)(*it)->GetDestID());
      debugMsg += " in NodeID ";
      debugMsg += std::to_string((int)owner->GetNodeId());
      debugMsg += " was set to active";
      comnet::debug::Log::Message(comnet::debug::LOG_DEBUG, debugMsg);
    }

    Void PingManager::TransferToInactivePingers(std::list <Pinger_Ptr>::iterator it)
    {
						if (!(*it)->IsSynced())
						{
								syncManager->RemoveUnsyncedPinger(*it);
						}
      inactivePingers->splice(inactivePingers->end(), *activePingers, it);
      std::string debugMsg = "Pinger with destID ";
      debugMsg += std::to_string((int)(*it)->GetDestID());
      debugMsg += " in NodeID ";
      debugMsg += std::to_string((int)owner->GetNodeId());
      debugMsg += " was set to inactive";
      comnet::debug::Log::Message(comnet::debug::LOG_DEBUG, debugMsg);
    }
  }
}