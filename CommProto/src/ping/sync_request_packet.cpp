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

#include <CommProto/ping/sync_request_packet.h>

namespace comnet
{
		namespace ping
		{
				SyncRequestPacket::SyncRequestPacket()
						:CHAIN_ABSPACKET(SyncRequestPacket), request(false), time(0)
				{
						
				}

				void SyncRequestPacket::Pack(REF_OBJECTSTREAM objOut)
				{
						objOut << time;
						uint8_t requestInt = (uint8_t)request;
						objOut << requestInt;
				}

				void SyncRequestPacket::Unpack(REF_OBJECTSTREAM objIn)
				{
						objIn >> time;
						uint8_t requestInt;
						objIn >> requestInt;
						request = (bool)requestInt;
				}

		}
}
