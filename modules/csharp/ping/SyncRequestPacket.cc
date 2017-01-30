#include <ping/SyncRequestPacket.h>

namespace Comnet {
		namespace Ping {
				SyncRequestPacket::SyncRequestPacket()
						:ABSPacket("SyncRequestPacket"), syncRequestNum(0)
				{

				}

				Void SyncRequestPacket::Pack(ObjectStream^ objOut)
				{
						objOut->Input(syncRequestNum);
				}

				Void SyncRequestPacket::Unpack(ObjectStream^ objIn)
				{
						syncRequestNum = (uint8_t)objIn->OutputByte();
				}
		}
}