#ifndef __CSHARP_PING_PACKET_H
#define __CSHARP_PING_PACKET_H

#include <vcclr.h>
#using <mscorlib.dll>
#using <System.dll>
#include <ABSPacket.h>

namespace Comnet {
		namespace Ping {
#pragma managed
				using namespace System;
				public ref class PingPacket : public ABSPacket
				{
				public:
						PingPacket();

						Void Pack(ObjectStream^ objOut) override;

						Void Unpack(ObjectStream^ objIn) override;

						ABSPacket^ Create() override
						{
								return gcnew PingPacket();
						}

						Boolean IsPing()
						{
								return ping;
						}

						Void SetPing(bool mode)
						{
								ping = mode;
						}

				private:
						bool ping;
				};
		}
}

#endif