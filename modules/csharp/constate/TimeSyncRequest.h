/*
Packet for syncing time.

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

#ifndef __CSHARP_TIME_SYNC_REQUEST_H
#define __CSHARP_TIME_SYNC_REQUEST_H

#include <vcclr.h>
#using <mscorlib.dll>
#using <System.dll>
#include <ObjectStream.h>
#include <ABSPacket.h>
#include <constate/ConnectionState.h>

namespace Comnet {
		namespace Constate {
#pragma managed
				using namespace System;
				/**
				Packet to be sent when requesting the timestamp of the peer.
				*/
				public ref class TimeSyncRequest : public ABSPacket
				{
				public:
						/**
						Creates a new instance of {@link SyncRequestPacket}.
						@return A new {@link SyncRequestPacket}.
						*/
						TimeSyncRequest()
								:ABSPacket("TimeSyncRequest"), unixTimeHighResTimeDif(0)
						{
						}

						/**
						Serialize the {@link #unixTimeHighResTimeDif}.
						@param objOut The stream to serialize the data to.
						*/
						Void Pack(ObjectStream^ objOut) override
						{
								objOut->Input((int64_t)unixTimeHighResTimeDif);
						}

						/**
						Deserialize the {@link #unixTimeHighResTimeDif}.
						@param objIn The stream to parse data from.
						*/
						Void Unpack(ObjectStream^ objIn) override
						{
								unixTimeHighResTimeDif = objIn->OutputInt64();
						}

						/**
						Set the {@link #unixTimeHighResTimeDif} to the difference between the two arguments.
						@param unixTimeMillis The unix time stamp
						@param highResTimeMillis The time value of the high resolution clock
						*/
						Void SetTimeDif(UnixMillisInt unixTimeMillis, UnixMillisInt highResTimeMillis) {
								this->unixTimeHighResTimeDif = unixTimeMillis - highResTimeMillis;
						}

						/**
						Accessor for {@link #unixTimeHighResTimeDif}.
						*/
						UnixMillisInt GetTimeDif() {
								return this->unixTimeHighResTimeDif;
						}

						/**
						Creates a new instance of {@link TimeSyncRequest}.
						@return New instance of {@link TimeSyncRequest}.
						*/
						ABSPacket^ Create() override {
								return gcnew TimeSyncRequest();
						}

						/**
						Default destructor.
						*/
						~TimeSyncRequest()
						{
						}

				private:
						/**
						The difference between the unix time stamp and the high resolution clock.
						*/
						UnixMillisInt unixTimeHighResTimeDif;
				};
		} //namespace ping
} //namespace comnet
#endif //__CSHARP_TIME_SYNC_REQUEST_H
