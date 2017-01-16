/*
Packet for syncing time

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

#ifndef __SYNC_REPLY_PACKET_H
#define __SYNC_REPLY_PACKET_H

#include <CommProto/abstractpacket.h>

namespace comnet {
		namespace ping {
				class SyncReplyPacket : INHERITS_ABSPACKET
				{
				public:
						SyncReplyPacket()
								: CHAIN_ABSPACKET(ABSPacket)
						{

						}

						void Pack(REF_OBJECTSTREAM objOut) override;

						void Unpack(REF_OBJECTSTREAM objIn) override;

						void SetRequestSentTime(int32_t requestSentTime)
						{
								this->requestSentTime = requestSentTime;
						}

						int32_t GetRequestSentTime()
						{
								return requestSentTime;
						}

						AbstractPacket* Create() override
						{
								return new SyncReplyPacket();
						}

				private:
						int32_t requestSentTime;
				};
		}
}

#endif
