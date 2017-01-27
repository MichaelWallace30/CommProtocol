#ifndef __CSHARP_SYNC_REQUEST_PACKET_H
#define __CSHARP_SYNC_REQUEST_PACKET_H

#include <vcclr.h>
#using <mscorlib.dll>
#using <System.dll>
#include <ABSPacket.h>

namespace Comnet {
		namespace Ping {
#pragma managed
				using namespace System;
				public ref class SyncRequestPacket : public ABSPacket
				{
				public:
						SyncRequestPacket();

						Void Pack(ObjectStream^ objOut) override;

						Void Unpack(ObjectStream^ objIn) override;

						ABSPacket^ Create() override
						{
								return gcnew SyncRequestPacket();
						}
				};
		}
}

#endif