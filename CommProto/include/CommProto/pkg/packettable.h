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

  namespace detail {
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
  }

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

  /**
    Iterates over the table and checks if the call parameter exists.
  */
  bool Contains(Callback* call);

  /**
    Accessor for the number of pairs that are inside the table.
  */
  uint32_t getNumOfPairs()
  {
    return numOfPairs;
  }

private:

  static const double LOAD_FACTOR;

  uint32_t FindArrPos(uint32_t key);
  /**
    Traverse the table.
   */  
  int32_t TraverseIndex(int32_t i);
  /**
    The benevolent key hasher. 
   */  
  uint32_t KeyHash(uint32_t key);

  /**
    Stores a pointer to a Pair that will be used as a flag for elements that were removed from the table.
    Must be allocated to guarantee that the space in memory can't be used by other elements.
  */
  static const detail::Pair* REMOVED_PTR;

  /**
    True if there is a valid object in the ptr, false otherwise
  */
  inline bool containsObject(detail::Pair* ptr)
  {
    return (ptr != NULL && ptr != REMOVED_PTR);
  }

  /**
    The table that houses AbstractPacket-Callback pairs.
   */
  detail::Pair** table;
  /**
    Keeps track of the number of pairs in the table.
   */
  uint32_t numOfPairs;
  /**
    Keeps track of the table size.
   */
  uint32_t tableSize;
  
  /**
    The maximum number of pairs the table can store before resizing
  */
  uint32_t tableCapacity;
};
} // namespace Pkg
} // namespace Comnet

#endif // __PACKET_TABLE_H
