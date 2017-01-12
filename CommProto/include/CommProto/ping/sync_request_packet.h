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

#ifndef __SYNC_REQUEST_PACKET_H
#define __SYNC_REQUEST_PACKET_H

#include <CommProto/abstractpacket.h>
#include <string>

namespace comnet {
		namespace ping {
				/**
				Packet to be sent and received to ping and pong.
				*/
				class SyncRequestPacket : INHERITS_ABSPACKET
				{
				public:
						/**
						Creates a new instance of {@link PingPacket}.
						@return A new {@link PingPacket}.
						*/
						SyncRequestPacket();

						/**
						Serializes {@link #ping} to the stream.
						@param objOut The stream to serialize the data to.
						*/
						void Pack(REF_OBJECTSTREAM objOut) override;

						/**
						Parses {@link #ping} from the stream.
						@param objIn The stream to parse data from.
						*/
						void Unpack(REF_OBJECTSTREAM objIn) override;

						/**
						Creates a new instance of {@link PingPacket}.
						@return New instance of {@link PingPacket}.
						*/
						AbstractPacket* Create() override
						{
								return new SyncRequestPacket();
						}

						void SetTime(int32_t time)
						{
								this->time = time;
						}

						void SetRequest(bool mode)
						{
								this->request = mode;
						}

						bool IsRequest()
						{
								return request;
						}

						int32_t GetTime()
						{
								return time;
						}

				private:
						int32_t time;
						bool request;
				};
		} //namespace ping
} //namespace comnet
#endif //__PING_PACKET_H
