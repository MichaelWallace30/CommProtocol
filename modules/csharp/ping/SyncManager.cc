#include <ping/SyncManager.h>
#include <ping/SyncRequestPacket.h>
#include <ping/SyncReplyPacket.h>
#include <Comms.h>

namespace Comnet {
		namespace Ping {
				Int32 SyncRequestCallback(Header^ header, ABSPacket^ absPacket, CommNode^ node)
				{
						SyncReplyPacket^ replyPacket = gcnew SyncReplyPacket();
						replyPacket->SetRequestSentTime(header->GetSourceTime());
						node->Send(replyPacket, header->GetSourceID());
						return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
				}

				Int32 SyncReplyCallback(Header ^ header, ABSPacket ^ absPacket, CommNode ^ node)
				{
						SyncReplyPacket^ syncReplyPacket = (SyncReplyPacket^)absPacket;
						Int32 estSendTime = (Pinger::GetTimeSinceStart() - syncReplyPacket->GetRequestSentTime()) / 2;
						Int32 estDestTimeStamp = header->GetSourceTime() + estSendTime;
					 Int32 timeOff = Pinger::GetTimeSinceStart() - estDestTimeStamp;
						((Comms^)node)->GetPingManager()->
				}

				SyncManager::SyncManager(Comms ^ owner)
				{
						
				}
		}
}