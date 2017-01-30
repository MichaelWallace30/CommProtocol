#include <ping/SyncRequestPacket.h>

namespace Comnet {
		namespace Ping {
				SyncRequestPacket::SyncRequestPacket()
						:ABSPacket("SyncRequestPacket"), unixTimeHighResTimeDif(0)
				{

				}

				Void SyncRequestPacket::Pack(ObjectStream^ objOut)
				{
						objOut->Input(unixTimeHighResTimeDif);
				}

				Void SyncRequestPacket::Unpack(ObjectStream^ objIn)
				{
						unixTimeHighResTimeDif = objIn->OutputInt64();
				}
		}
}