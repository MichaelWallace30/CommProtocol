/*
  PacketFactory that creates packets in the library.

  Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia, Alex Craig.

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
#ifndef __PACKET_FACTORY_H
#define __PACKET_FACTORY_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/macros.h>

#include <CommProto/abstractpacket.h>

namespace comnet {
namespace pkg {


/**
   PacketFactory will be allow to handle the creation of the 
   Packet.
*/
class COMM_EXPORT PacketFactory {
public:  
  /**
     Constructor for the Packet Factory.

   */
  PacketFactory() { }
  /**
     Produces a packet from the associated table.
   */
  AbstractPacket* ProduceNewPacket(AbstractPacket* ref) { 
    if (!ref) {
      return NULL;
    }

    return ref->Create(); 
  }
};
} // namespace Pkg
} // namespace Comnet
#endif // __PACKET_FACTORY_H
