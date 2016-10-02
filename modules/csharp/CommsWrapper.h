/*
Copyright (C) 2016  Michael Wallace, Mario Garcia.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once
#define _DEBUG 1
#include <CommProto/comms.h>
#include <CommProto/packets.h>
#include <CommProto/architecture/connection/transport_type.h>

#include <ABSPacket.h>
#include <CallBack.h>
#include <network/TransportProtocol.h>

#using <mscorlib.dll>

using namespace System;
using namespace Comnet::Network;

namespace Comnet {
#pragma managed

	public ref class CommsWrapper sealed
	{
	private:
		Comms *unmangedComms;
		Ping *ping;

	public:
		CommsWrapper(uint8_t platformID);
		~CommsWrapper();


		bool initConnection(TransportProtocol connectionType, String^ port, String^ address, uint32_t baudrate);


		bool addAddress(uint8_t destID, String^ address, uint16_t port);


		bool removeAddress(uint8_t destID);

		bool send(ABSPacket^ packet, uint8_t destID);			//issue
		bool receive(ABSPacket^ p, uint8_t%  sourceID);					//issue

		bool linkCallback(const ABSPacket^ packet, const CallBack^ callback);//issue

		int32_t run();
		int32_t pause();
		int32_t stop();

	};
}//end namespace

