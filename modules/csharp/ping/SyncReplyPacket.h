#ifndef __CSHARP_SYNC_REPLY_PACKET_H
#define __CSHARP_SYNC_REPLY_PACKET_H

#include <vcclr.h>
#using <mscorlib.dll>
#using <System.dll>
#include <ABSPacket.h>

namespace Comnet {
		namespace Ping {
#pragma managed
				using namespace System;
				public ref class SyncReplyPacket : public ABSPacket
				{
				public:
						SyncReplyPacket();

						Void Pack(ObjectStream^ objOut) override;

						Void Unpack(ObjectStream^ objIn) override;

						Void SetTimeDif(Int64 unixTimeMillis, Int64 highResTimeMillis)
						{
								unixTimeHighResTimeDif = unixTimeMillis - highResTimeMillis;
						}

						Int64 GetTimeDif()
						{
								return unixTimeHighResTimeDif;
						}

					 Int32 GetRequestSentTime()
						{
								return requestSentTime;
						}

						Void SetRequestSentTime(int32_t requestSentTime)
						{
								this->requestSentTime = requestSentTime;
						}

						ABSPacket^ Create() override
						{
								return gcnew SyncReplyPacket();
						}

				private:
						Int32 requestSentTime;

						Int64 unixTimeHighResTimeDif;
				};
		}
}

#endif