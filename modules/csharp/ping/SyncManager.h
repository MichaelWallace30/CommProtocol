#ifndef __CSHARP_SYNC_MANAGER_H
#define __CSHARP_SYNC_MANAGER_H

#include <gcroot.h>
#include <list>
#include <vcclr.h>
#using <mscorlib.dll>
#using <System.dll>
#include <Header.h>
#include <CallBack.h>
#include <ping/Pinger.h>

namespace Comnet {
		ref class Comms;
		namespace Ping {
				ref class SyncRequestPacket;
				ref class SyncReplyPacket;

				using namespace System::Collections::Generic;
#pragma managed
				Int32 SyncRequestCallback(Header^ header, ABSPacket^ absPacket, CommNode^ node);
				Int32 SyncReplyCallback(Header^ header, ABSPacket^ absPacket, CommNode^ node);

				public ref class SyncManager {
				public:
						typedef gcroot<Pinger^> Pinger_Ptr;

						SyncManager(Comms^ owner);

						Void SyncSendHandler();

						Void AddUnsyncedPinger(Pinger_Ptr pinger);

						Void RemoveUnsyncedPinger(Pinger_Ptr pinger);

						Void SyncTime(Pinger_Ptr pinger, Int32 time);

						Void SendSyncRequest(Pinger_Ptr destPinger);

						Void LinkCallbacks();

						Void Run();

						Void Stop();

						~SyncManager();

						!SyncManager();

				private:
						std::list <Pinger_Ptr>* unsyncedPingers;
						Threading::AutoResetEvent^ syncHandlerRE;
						Threading::Mutex^ runningMutex;
						Threading::Mutex^ unsyncedPingerMutex;
						Threading::Thread^ syncSendThread;
						SyncRequestPacket^ syncRequestPack;
						SyncReplyPacket^ syncReplyPack;
						Comms^ owner;
						bool running;
				};
		}
}
#endif