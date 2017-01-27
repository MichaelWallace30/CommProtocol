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

					 Int32 GetRequestSentTime()
						{
								return requestSentTime;
						}

						Void SetRequestSentTime(int32_t requestTimeSent)
						{
								this->requestSentTime = requestSentTime;
						}

						ABSPacket^ Create() override
						{
								return gcnew SyncReplyPacket();
						}

				private:
						Int32 requestSentTime;
				};
		}
}

#endif