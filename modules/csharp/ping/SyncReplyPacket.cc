#include <ping/SyncReplyPacket.h>

namespace Comnet {
		namespace Ping {
				SyncReplyPacket::SyncReplyPacket()
						:ABSPacket("SyncReplyPacket"), requestSentTime(0)
				{

				}

				Void SyncReplyPacket::Pack(ObjectStream^ objOut)
				{
						objOut->Input(requestSentTime);
						objOut->Input(unixTimeHighResTimeDif);
				}

				Void SyncReplyPacket::Unpack(ObjectStream^ objIn)
				{
						unixTimeHighResTimeDif = objIn->OutputInt64();
						requestSentTime = objIn->OutputInt32();
				}
		}
}