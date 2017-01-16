/*
Represents a client that needs to be pinged to check if still connected.

Copyright (C) 2016  Alex Craig, Michael Wallace, Mario Garcia.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(At your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __SYNC_MANAGER_H
#define __SYNC_MANAGER_H

#include <CommProto/callback.h>
#include <CommProto/architecture/os/comm_thread.h>
#include <mutex>
#include <list>
#include <memory>

namespace comnet
{
		class Comms;
namespace ping
{
		using namespace comnet::architecture::os;

		class Pinger;
		class SyncRequestPacket;
		class SyncReplyPacket;

		error_t SyncRequestCallback(const comnet::Header& header, SyncRequestPacket& packet, comnet::Comms& node);

		error_t SyncReplyCallback(const comnet::Header& header, SyncReplyPacket& packet, comnet::Comms& node);
		class SyncManager : public std::enable_shared_from_this<SyncManager>
		{
		public:
				SyncManager(Comms* owner);

				void SyncSendHandler();

				void AddUnsyncedPinger(Pinger* pinger);

				void RemoveUnsyncedPinger(Pinger* pinger);

				void SyncTime(Pinger* pinger, int32_t time);

				void SendSyncRequest(uint8_t destID);

				void LinkCallbacks();

				void Run();

				void Stop();

				~SyncManager();

		private:
				std::condition_variable syncHandlerCV;
				std::mutex syncHandlerMutex;
				std::mutex unsyncedPingersMutex;
				std::mutex runningMutex;
				CommThread* syncSendThread;

				std::list <Pinger*> unsyncedPingers;

				bool awake;

				bool running;

				SyncRequestPacket* syncRequestPack;
				SyncReplyPacket* syncReplyPack;

				Comms* owner;
		};
}
}

#endif
