/*
  Packet Manager, handles an ADT storage for associating Packets.

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
#ifndef __PACKET_MANAGER_H
#define __PACKET_MANAGER_H


#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/pkg/packetfactory.h>


namespace comnet {


class AbstractPacket;
class Callback;


namespace pkg {


class PacketTable;

/**
   PackageManager is an ADT, designed specifically to implement
   packets and associate them to Callbacks. If no callback is associated with
   a Packet, null should be returned.
*/
class COMM_EXPORT PacketManager {
public:

  /**
     Default package manager.
   */
  PacketManager();

  /**
     Declare a specific size for the table, before constuction.
   */
  PacketManager(uint32_t set_size);

  /**
     Default Destructor.
   */
  ~PacketManager();

  /**
     Inserts an object into the PackageManager, simply call:
     
     insert(new AbstractPacket()(has to be specialized though), new Callback(funct));

   */
  bool Insert(const AbstractPacket* key, const Callback* callback);

  /**
     Gets the callback associated with the AbstractPacket.
   */
  Callback* Get(const AbstractPacket& key);

  /**
     Removes the Callback associated with key.
   */
  bool Remove(const AbstractPacket& key);

  /**
     Check if the table Contains this AbstractPacket.
   */
  bool Contains(const AbstractPacket& key);

  /**
     Check if the table containes this callback.
   */
  bool Contains(Callback* call);

  /**
    Get the number of Packet-callback pairs in this table.
  */
  uint32_t GetSize();

  /**
    Allocates more space for the hashtable.
  */
  bool Reserve(uint32_t size);

  /**
     Will produce a Packet from the provided id.
   */
  AbstractPacket* ProduceFromId(uint32_t key);
private:

  PacketFactory factory;

  PacketTable* table;
};
} // namespace Pkg
} // namespace Comnet
#endif // __PACKET_MANAGER_H
