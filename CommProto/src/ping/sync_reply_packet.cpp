#include <CommProto/ping/sync_reply_packet.h>

namespace comnet {
		namespace ping {
				void SyncReplyPacket::Pack(REF_OBJECTSTREAM objOut)
				{
						objOut << requestSentTime;
						objOut << unixTimeHighResTimeDif;
				}
				void SyncReplyPacket::Unpack(REF_OBJECTSTREAM objIn)
				{
						objIn >> unixTimeHighResTimeDif;
						objIn >> requestSentTime;
				}
		}
}