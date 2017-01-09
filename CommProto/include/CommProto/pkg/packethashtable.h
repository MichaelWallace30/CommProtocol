/*
Packet Table, deals with storage for associating Packets and Callbacks.

Copyright (C) 2016  Alex Craig, Michael Wallace, Kartik Soni, Mario Garcia.

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
#ifndef __PACKET_HASH_TABLE_H
#define __PACKET_HASH_TABLE_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/macros.h>
#include <CommProto/pkg/packettable.h>

namespace comnet {

  class AbstractPacket;
  class Callback;

  namespace pkg {

    //Namespace to hide the Pair implementation.  This class can no longer be nested because a static instance must be created.
    namespace detail {
      /**
        Holds AbstractPacket and Callback pointer in the hashtable.
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
      An Open Address Hash Table that uses AbstractPacket as the key and Callback as the value.
     */
    class COMM_EXPORT PacketHashTable : public PacketTable {
      COMM_DISALLOW_COPYING(PacketHashTable);
    public:
      /**
        Constructs a HashTable and sets the size to the value of the parameter.
       */
      PacketHashTable(uint32_t setSize);
      /**
        Constructs a HashTable and sets the size to 
      */
      PacketHashTable();
      /**
        Default destructor.
       */
      ~PacketHashTable();
      /**
        Inserts a Packet with a Callback associated with it.
       */
      bool Insert(const AbstractPacket* key, const Callback* callback) override;
      /**
        Returns the callback with the associated distinct key from the packet.
       */
      Callback* GetCallback(uint32_t key) override;
      /**
        Grab the packet that is associated with the key.
       */
      AbstractPacket* GetPacket(uint32_t key) override;
      /**
        Remove a packet-Callback pair in the table.
       */
      bool Remove(uint32_t key) override;
      /**
        Changes the size of the table to the size of the parameter, when the size of the parameter is greater.
       */
      bool Reserve(uint32_t new_size) override;
      /**
        Iterates over the table and checks if the call parameter exists.
       */
      bool Contains(Callback* call) override;
      /**
        Accessor for the number of pairs that are inside the table.
       */
      uint32_t getNumOfPairs() override
      {
        return numOfPairs;
      }

    private:
      /**
        The initial size of the HashTable when the default constructor is used.
       */
      static const int DEFAULT_TABLE_SIZE = 16;

      /**
        Multiplied to the tableSize to set the tableCapacity.
       */
      static const double LOAD_FACTOR;
      /**
        Stores a pointer to a Pair that will be used as a flag for elements that were removed from the table.
        Must be allocated to guarantee that the space in memory can't be used by other elements.
       */
      static const detail::Pair* REMOVED_PTR;
     
      /**
        True if there is a valid object in the ptr, false otherwise.
       */
      inline bool containsObject(detail::Pair* ptr)
      {
        return (ptr != NULL && ptr != REMOVED_PTR);
      }
      /**
        Returns the table index of the key, will return an index pointing to NULL if the key is not found.
       */
      uint32_t FindTableIndex(uint32_t key);
      /**
        Traverses the index through the table, increments by 1 and resets index to 0 when end is reached.
       */
      uint32_t TraverseIndex(uint32_t i);
      /**
        Returns a hash of the key parameter.
       */
      uint32_t KeyHash(uint32_t key);

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
        The maximum number of pairs the table can store before resizing.
       */
      uint32_t tableCapacity;
    };
  } // namespace Pkg
} // namespace Comnet

#endif // __PACKET_TABLE_H
