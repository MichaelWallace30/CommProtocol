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
				}

				Void SyncReplyPacket::Unpack(ObjectStream^ objIn)
				{
						requestSentTime = objIn->OutputInt32();
				}
		}
}