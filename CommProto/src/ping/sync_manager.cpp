#include <CommProto/ping/sync_manager.h>
#include <CommProto/ping/sync_reply_packet.h>
#include <CommProto/ping/sync_request_packet.h>
#include <CommProto/ping/pinger.h>
#include <CommProto/comms.h>

namespace comnet {
		namespace ping {
				error_t SyncRequestCallback(const comnet::Header & header, SyncRequestPacket & packet, comnet::Comms & node)
				{
						SyncReplyPacket replyPacket;
						replyPacket.SetRequestSentTime(header.GetSourceTime());
						replyPacket.SetTimeDif(Pinger::GetUnixTimeMillis(), Pinger::GetTimeSinceStart());
						node.Send(replyPacket, header.source_id);
						node.GetPingManager()->CheckResync(header.source_id, packet.GetTimeDif());
						return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
				}

				error_t SyncReplyCallback(const comnet::Header & header, SyncReplyPacket & packet, comnet::Comms & node)
				{
						int32_t estSendTime = (Pinger::GetTimeSinceStart() - packet.GetRequestSentTime()) / 2;
						int32_t estDestTimeStamp = header.GetSourceTime() + estSendTime;
						int32_t timeOff = Pinger::GetTimeSinceStart() - estDestTimeStamp;
						node.GetPingManager()->CheckResync(header.source_id, packet.GetTimeDif());
						node.GetPingManager()->SyncTime(header.source_id, timeOff);
						return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
				}

				SyncManager::SyncManager(Comms* owner)
						:owner(owner), syncSendThread(nullptr)
				{
						syncRequestPack = new SyncRequestPacket();
						syncReplyPack = new SyncReplyPacket();
				}

				void SyncManager::SyncSendHandler()
				{
						while (true)
						{
								runningMutex.lock();
								if (!running)
								{
										runningMutex.unlock();
										return;
								}
								unsyncedPingersMutex.lock();
								awake = false;
								if (!unsyncedPingers.empty())
								{
										std::list <Pinger*>::iterator it;
										for (it = unsyncedPingers.begin(); it != unsyncedPingers.end(); it++)
										{
												if ((*it)->GetNextSyncTimeMillis() <= 0)
												{
														SendSyncRequest((*it)->GetDestID());
														(*it)->ResetSyncPackSentTime();
												}
												else
												{
														break;
												}
										}
										if (it != unsyncedPingers.end())
										{
											 unsyncedPingers.splice(unsyncedPingers.end(), unsyncedPingers, unsyncedPingers.begin(), it);
										}
										MillisInt sleepTime = unsyncedPingers.front()->GetNextSyncTimeMillis();
										std::unique_lock<std::mutex> syncHandleLock(syncHandlerMutex);
										runningMutex.unlock();
										unsyncedPingersMutex.unlock();
										syncHandlerCV.wait_for(syncHandleLock, (std::chrono::milliseconds)sleepTime, [&] {return awake; });
								}
								else
								{
										std::unique_lock<std::mutex> syncHandleLock(syncHandlerMutex);
										unsyncedPingersMutex.unlock();
										runningMutex.unlock();
										syncHandlerCV.wait(syncHandleLock, [&] {return awake; });
								}
						}
				}

				void SyncManager::AddUnsyncedPinger(Pinger * pinger)
				{
						std::cout << "UNSYNCED PINGER ADDED" << std::endl;
						unsyncedPingersMutex.lock();
						unsyncedPingers.push_front(pinger);
						unsyncedPingersMutex.unlock();
						{
								std::unique_lock <std::mutex> syncHandlerLock(syncHandlerMutex);
								awake = true;
						}
						syncHandlerCV.notify_one();
						pinger->SetInUnsyncedList(true);
				}

				void SyncManager::RemoveUnsyncedPinger(Pinger * pinger)
				{
						std::cout << "UNSYNCED PINGER REMOVED" << std::endl;
						std::unique_lock <std::mutex> lock(unsyncedPingersMutex);
						unsyncedPingers.remove(pinger);
						pinger->SetInUnsyncedList(false);
				}

				void SyncManager::SyncTime(Pinger * pinger, int32_t timeOff)
				{
						pinger->SyncTime(timeOff);
						if (pinger->IsSynced())
						{
								RemoveUnsyncedPinger(pinger);
						}
				}

				void SyncManager::SendSyncRequest(uint8_t destID)
				{
						SyncRequestPacket syncRequest;
						syncRequest.SetTimeDif(Pinger::GetUnixTimeMillis(), Pinger::GetTimeSinceStart());
						owner->Send(syncRequest, destID);
				}

				void SyncManager::LinkCallbacks()
				{
						owner->LinkCallback(syncRequestPack, new comnet::Callback((comnet::callback_t)SyncRequestCallback));
						owner->LinkCallback(syncReplyPack, new comnet::Callback((comnet::callback_t)SyncReplyCallback));
				}

				bool SyncManager::Run()
				{
						running = true;
						syncSendThread = new CommThread(&SyncManager::SyncSendHandler, shared_from_this());
						syncSendThread->Detach();
						return true;
				}

				void SyncManager::Stop()
				{
						runningMutex.lock();
						running = false;
						runningMutex.unlock();
						{
								std::unique_lock <std::mutex> syncHandlerLock(syncHandlerMutex);
								awake = true;
						}
						syncHandlerCV.notify_one();
				}

				SyncManager::~SyncManager()
				{
						free_pointer(syncSendThread);
						free_pointer(syncRequestPack);
						free_pointer(syncReplyPack);
				}
		}
}