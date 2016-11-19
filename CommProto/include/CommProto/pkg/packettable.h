/*
  Packet Table, deals with storage for associating Packets and Callbacks.

  Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia.

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
#ifndef __PACKET_TABLE_H
#define __PACKET_TABLE_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/macros.h>


namespace comnet {

class AbstractPacket;
class Callback;

namespace pkg {

/**
  Normal Packet table used for mapping packets in association of 
  Callbacks with Packets.
*/
class COMM_EXPORT PacketTable {
public:
  /**
    Constructor of set size.
   */
  PacketTable(uint32_t setSize);
  /**
    Default constructor.
   */
  PacketTable();
  /**
    Default destructor.
   */
  ~PacketTable();
  /**
    Inserts a Packet with a Callback associated with it.
   */
  bool Insert(const AbstractPacket* key, const Callback* callback);
  /**
    Returns the callback with the associated distinct key from the packet.
   */
  Callback* GetCallback(uint32_t key);
  /**
    Grab the packet that is associated with the key.
   */
  AbstractPacket* GetPacket(uint32_t key);
  /**
    Remove a packet-Callback pair in the table.
   */
  bool Remove(uint32_t key);
  /**
    Resize the table when needed, or if the table is getting to big.
   */
  bool Resize(uint32_t new_size);

private:
  /**
    Traverse the table.
   */  
  int32_t TraverseIndex(int32_t i);
  /**
    The benevolent key hasher. 
   */  
  uint32_t KeyHash(uint32_t key);
  /**
    Creates a pair.
   */
  struct Pair {
    /**
      The callback associated with this pair.
     */
    Callback* callback;
    /**
      The packet associated with this pair.
     */
    AbstractPacket* packet;
  };
  /**
    The table that houses AbstractPacket-Callback pairs.
   */
  Pair** table;
  /**
    Keeps track of the number of pairs in the table.
   */
  uint32_t numOfPairs;
  /**
    Keeps track of the table size.
   */
  uint32_t tableSize;
};
} // namespace Pkg
} // namespace Comnet

#endif // __PACKET_TABLE_H
