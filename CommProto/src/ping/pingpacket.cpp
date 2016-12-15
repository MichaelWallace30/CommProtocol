/*
		Packet for sending and receiving pings

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

#include <CommProto/ping/pingpacket.h>
#include <CommProto/serialization/objectstream.h>
#include <iostream>

namespace comnet {
namespace ping {

PingPacket::PingPacket()
  :CHAIN_ABSPACKET(PingPacket)
{
}

void PingPacket::Pack(REF_OBJECTSTREAM objOut)
{
		uint8_t pingNum = ping;
		objOut << pingNum;
}

void PingPacket::Unpack(REF_OBJECTSTREAM objIn)
{
		uint8_t pingNum;
		objIn >> pingNum;
		ping = (bool)pingNum;
}

PingPacket::~PingPacket()
{
}

} //namespace ping
} //namespace comnet
