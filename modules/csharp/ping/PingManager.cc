#include <ping/PingManager.h>
#include <ping/PingPacket.h>
#include <Comms.h>

namespace Comnet {
		namespace Ping {
			 Int32 PingCallback(Header^ header, ABSPacket^ absPacket, CommNode^ node) {
						PingPacket^ packet = static_cast<PingPacket^>(absPacket);
						Comms^ comms = static_cast<Comms^>(node);
						
						if (packet->IsPing())
						{
								/*
								std::string str = "PING RECEIVED BY ";
								str += std::to_string((int)comms->GetNodeId());
								str += " SENT FROM ";
								str += std::to_string((int)header->GetSourceID());
								str += "\n";
								COMMS_DEBUG(str.c_str());
								*/
								if (comms->GetPingManager()->CanPong(header->GetSourceID()))
								{
										/*
										str = "PONG SENT TO ";
										str += std::to_string((int)header->GetSourceID());
										str += " FROM ";
										str += std::to_string((int)comms->GetNodeId());
										str += "\n";
										COMMS_DEBUG(str.c_str());
										*/
										PingPacket^ pingPacket = gcnew PingPacket();
										pingPacket->SetPing(false);
										comms->Send(pingPacket, header->GetSourceID());
								}
						}
						else
						{
								/*
								std::string str = "PONG RECEIVED BY ";
								str += std::to_string((int)comms->GetNodeId());
								str += " SENT FROM ";
								str += std::to_string((int)header->GetSourceID());
								str += "\n";
								COMMS_DEBUG(str.c_str());
								*/
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
				}

				Void PingManager::LinkPingCallback()
				{
						owner->LinkCallback(pingPacket, gcnew CallBack(gcnew CommFunct(Comnet::Ping::PingCallback)));
				}

				Boolean PingManager::Run()
				{
						running = true;
						pingSendThread = gcnew Threading::Thread(gcnew Threading::ThreadStart(this, &PingManager::HandlePingUpdate));
						pingSendThread->Start();
						return true;
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

				Void PingManager::ResetPingTime(uint8_t destID)
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
								Console::WriteLine("iter1");
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
										Console::WriteLine("iter2");
										while (it != activePingers->end())
										{
												Console::WriteLine("iter3");
												//Gets the amount of milliseconds until the pinger needs to be sent to
												MillisInt nextPingTime = (*it)->GetNextPingTimeMillis();
												//When nextPingTime is less than 0 that means its ready to be sent a ping
												if (nextPingTime <= 0)
												{
														Console::WriteLine("iter4");
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
														Console::WriteLine("iter5");
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
								Console::WriteLine("iter6");
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
						activePingersMutex->ReleaseMutex();
				}

				Void PingManager::RemovePinger(uint8_t destID)
				{
						destPingerMapMutex->WaitOne();
						auto mapIter = destPingerMap->find(destID);
						if (mapIter != destPingerMap->end())
						{
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
						/*
						std::string str = "PING SENT TO ";
						str += std::to_string((int)destID);
						str += " FROM ";
						str += std::to_string((int)owner->GetNodeId());
						str += "\n";
						COMMS_DEBUG(str.c_str());
						*/
						//owner->Send(sendPacket, destID);
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