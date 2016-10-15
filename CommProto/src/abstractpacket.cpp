/*
  Abstract Packet implementation.

  Copyright (C) 2016  Mario Garcia.

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
#include <CommProto/abstractpacket.h>


// NOTE(): We will need to check this because g++ does not have functional like visual studio.
//#include <tr1/functional>
#include <CommProto/hash/ngcphash.h>
#include <CommProto/architecture/macros.h>

namespace comnet {


/**
   Constructor for AbstractPacket.
*/
AbstractPacket::AbstractPacket(const char* className)
: type_id(hash::ngcp_hash64((const char*)className) % BIT16_VALUE)
  { }


/**
   AbstractPacket Destructor.
*/
AbstractPacket::~AbstractPacket() 
  { }
} // Comnet
