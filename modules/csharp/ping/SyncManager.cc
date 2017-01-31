#include <ping/SyncManager.h>
#include <ping/SyncRequestPacket.h>
#include <ping/SyncReplyPacket.h>
#include <Comms.h>
#include <iostream>

namespace Comnet {
		namespace Ping {
				Int32 SyncRequestCallback(Header^ header, ABSPacket^ absPacket, CommNode^ node)
				{
						SyncRequestPacket^ requestPacket = (SyncRequestPacket^)absPacket;
						SyncReplyPacket^ replyPacket = gcnew SyncReplyPacket();
						replyPacket->SetRequestSentTime(header->GetSourceTime());
						replyPacket->SetTimeDif(Pinger::GetUnixTimeMillis(), Pinger::GetTimeSinceStart());
						std::cout << "HEADER SOURCE TIME: " << header->GetSourceTime() << std::endl;
						node->Send(replyPacket, header->GetSourceID());
						((Comms^)node)->GetPingManager()->CheckResync(header->GetSourceID(), requestPacket->GetTimeDif());
						return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
				}

				Int32 SyncReplyCallback(Header ^ header, ABSPacket ^ absPacket, CommNode ^ node)
				{
						SyncReplyPacket^ syncReplyPacket = (SyncReplyPacket^)absPacket;
						std::cout << "SyncReplyTime: " << syncReplyPacket->GetRequestSentTime() << std::endl;
						float estSendTime = (float)(Pinger::GetTimeSinceStart() - syncReplyPacket->GetRequestSentTime()) / 2.0f;
						float estDestTimeStamp = header->GetSourceTime() + estSendTime;
					 int32_t timeOff = (int32_t)(Pinger::GetTimeSinceStart() - estDestTimeStamp);
						((Comms^)node)->GetPingManager()->CheckResync(header->GetSourceID(), syncReplyPacket->GetTimeDif());
						((Comms^)node)->GetPingManager()->SyncTime(header->GetSourceID(), timeOff);
						return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
				}

				SyncManager::SyncManager(Comms ^ owner)
						:owner(owner), syncSendThread(nullptr), running(false)
				{
						unsyncedPingers = new std::list<Pinger_Ptr>();
						syncHandlerRE = gcnew Threading::AutoResetEvent(false); //False indicates that the reset event is not yet signalled
						runningMutex = gcnew Threading::Mutex();
						unsyncedPingerMutex = gcnew Threading::Mutex();
						syncRequestPack = gcnew SyncRequestPacket();
						syncReplyPack = gcnew SyncReplyPacket();
				}

				Void SyncManager::SyncSendHandler()
				{
						while (true)
						{
								runningMutex->WaitOne();
								if (!running)
								{
										runningMutex->ReleaseMutex();
										return;
								}
								unsyncedPingerMutex->WaitOne();
								if (!unsyncedPingers->empty())
								{
										std::list <Pinger_Ptr>::iterator it;
										for (it = unsyncedPingers->begin(); it != unsyncedPingers->end(); it++)
										{
												if ((*it)->GetNextSyncTimeMillis() <= 0)
												{
														SendSyncRequest(*it);
														(*it)->ResetSyncPackSentTime();
												}
												else
												{
														break;
												}
										}
										if (it != unsyncedPingers->end())
										{
												unsyncedPingers->splice(unsyncedPingers->end(), *unsyncedPingers, unsyncedPingers->begin(), it);
										}
										unsyncedPingerMutex->ReleaseMutex();
										MillisInt sleepTime = unsyncedPingers->front()->GetNextSyncTimeMillis();
										syncHandlerRE->WaitOne(sleepTime);
								}
								else
								{
										unsyncedPingerMutex->ReleaseMutex();
										syncHandlerRE->WaitOne();
								}
						}
				}

				Void SyncManager::AddUnsyncedPinger(Pinger_Ptr pinger)
				{
						unsyncedPingerMutex->WaitOne();
						unsyncedPingers->push_front(pinger);
						unsyncedPingerMutex->ReleaseMutex();
						syncHandlerRE->Set();
				}

				Void SyncManager::RemoveUnsyncedPinger(Pinger_Ptr pinger)
				{
						std::cout << "REMOVE CALLED" << std::endl;
						unsyncedPingerMutex->WaitOne();
						for (auto it = unsyncedPingers->begin(); it != unsyncedPingers->end(); it++)
						{
								if ((*it)->GetDestID() == pinger->GetDestID())
								{
										std::cout << "REMOVE SUCCESFULL" << std::endl;
										unsyncedPingers->erase(it);
										break;
								}
						}
						unsyncedPingerMutex->ReleaseMutex();
				}

				Void SyncManager::SyncTime(Pinger_Ptr pinger, int32_t timeOff)
				{
						std::cout << "SYNC TIME CALLED: " << timeOff << std::endl;
						pinger->SyncTime(timeOff);
						if (pinger->IsSynced()) {
								RemoveUnsyncedPinger(pinger);
						}
				}

				Void SyncManager::SendSyncRequest(Pinger_Ptr destPinger)
				{
						std::cout << "SYNC REQUEST SENT" << std::endl;
						SyncRequestPacket^ syncRequest = gcnew SyncRequestPacket();
						syncRequest->SetTimeDif(Pinger::GetUnixTimeMillis(), Pinger::GetTimeSinceStart());
						owner->Send(syncRequest, destPinger->GetDestID());
				}

				Void SyncManager::LinkCallbacks()
				{
						owner->LinkCallback(syncRequestPack, gcnew CallBack(gcnew CommFunct(Comnet::Ping::SyncRequestCallback)));
						owner->LinkCallback(syncReplyPack, gcnew CallBack(gcnew CommFunct(Comnet::Ping::SyncReplyCallback)));
				}

				Void SyncManager::Run()
				{
						running = true;
						syncSendThread = gcnew Threading::Thread(gcnew Threading::ThreadStart(this, &SyncManager::SyncSendHandler));
						syncSendThread->Start();
				}

				Void SyncManager::Stop()
				{
						runningMutex->WaitOne();
						running = false;
						runningMutex->ReleaseMutex();
						syncHandlerRE->Set();
				}

				SyncManager::~SyncManager() {
						this->!SyncManager();
				}

				SyncManager::!SyncManager() {
						delete unsyncedPingers;
						unsyncedPingers = nullptr;
				}
		}
}