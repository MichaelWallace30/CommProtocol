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
#ifndef __PING_PACKET_H
#define __PING_PACKET_H

#include <CommProto/abstractpacket.h>

namespace comnet {
namespace ping {
class PingPacket : INHERITS_ABSPACKET
{
public:
  PingPacket();
  ~PingPacket();
};
} //namespace ping
} //namespace comnet
#endif //__PING_PACKET_H
