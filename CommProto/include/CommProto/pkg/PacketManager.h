/*
  Packet Manager, handles an ADT storage for associating Packets.

  Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia.

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
#ifndef __PACKET_MANAGER_H
#define __PACKET_MANAGER_H

#include <CommProto/AbstractPacket.h>
#include <CommProto/Callback.h>

namespace Comnet {
namespace Pkg {

/**
   PackageManager is a hashtable, designed specifically to implement
   packets and associate them to Callbacks. If no callback is associated with
   a callback, null should be returned.
*/
class PackageManager {
public:
  
private:
  
};

} // namespace Pkg
} // namespace Comnet
#endif // __PACKET_MANAGER_H
