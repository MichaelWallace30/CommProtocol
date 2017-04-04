/*
PacketHashTable, stores an AbstractPacket and Callback pair in a dynamically sized openaddress hashmap.

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
#include <CommProto/pkg/packethashtable.h>
#include <CommProto/architecture/macros.h>
#include <CommProto/abstractpacket.h>
#include <CommProto/callback.h>


#if (COMMPROTO_TARGET_OS != COMMMPROTO_OS_WINDOWS) && (COMMPROTO_TARGET_OS != COMMPROTO_OS_APPLE)
#include <tr1/functional>
#define COMMSTD std::tr1
#else 
#include <functional>
#define COMMSTD std
#endif

namespace comnet {
  namespace pkg {

    using namespace detail;

    const double PacketHashTable::LOAD_FACTOR = .75;

    const Pair* PacketHashTable::REMOVED_PTR = new Pair();
 
    PacketHashTable::PacketHashTable(uint32_t setSize)
      : table(new Pair*[setSize])
      , numOfPairs(0)
      , tableSize(setSize)
      , tableCapacity(static_cast<uint32_t>(tableSize * LOAD_FACTOR))
    {

      nullifyAttributesInTable(table, tableSize);

    }


    PacketHashTable::PacketHashTable()
      : table(new Pair*[DEFAULT_TABLE_SIZE])
      , numOfPairs(0)
      , tableSize(DEFAULT_TABLE_SIZE)
      , tableCapacity(static_cast<uint32_t>(tableSize * LOAD_FACTOR))
    {

      nullifyAttributesInTable(table, tableSize);

    }


    PacketHashTable::~PacketHashTable()
    {
      Pair* pair;
      for (int32_t i = 0; i < tableSize; ++i) {
        pair = *(table + i);

        if (containsObject(pair)) {
          if (pair->packet) {
            delete pair->packet;
            pair->packet = NULL;
          }

          if (pair->callback) {
            delete pair->callback;
            pair->callback = NULL;
          }

          delete pair;
          pair = NULL;
        }
      }

      delete[] table;
    }


    bool PacketHashTable::Insert(const AbstractPacket* key, const Callback* callback) {
      //Return false if NULL was passed in as a key
      if (!key) {
        return false;
      }

      //If adding this element will exceed the tableCapacity, increase the size of the table.
      if (numOfPairs + 1 >= tableCapacity) {
        Reserve(tableSize * 2 + 1);
      }

      Pair* pair = allocate_pointer(Pair);
      pair->packet = (AbstractPacket*)key;
      pair->callback = (Callback*)callback;

      uint32_t hash = KeyHash(key->GetId());
      uint32_t savedHashTableIndex = KeyHash(pair->packet->GetId());		//I don't think we need this, it should be impossible to hit
      uint32_t tableIndex = savedHashTableIndex;		//The index of the array
      uint32_t removePtrTableIndex = tableSize;	//Saves the first pointer to the REMOVE_PTR that was iterated over

       //Loop to continue iterating over the table until a valid position is found
      while (*(table + tableIndex) == REMOVED_PTR || (*(table + tableIndex) != NULL && (*(table + tableIndex))->packet->GetId() != pair->packet->GetId()))
      {
        //If removePtrTableIndex hasn't been set yet and the index points to a REMOVED_PTR, set removePtrTableIndex to this value
        if (*(table + tableIndex) == REMOVED_PTR && removePtrTableIndex == tableSize)
        {
          removePtrTableIndex = tableIndex;
        }
        //Increments the tableIndex and will reset it to 0 if it exceeds the table's size
        tableIndex = TraverseIndex(tableIndex);
        //Only true after all values in the map have been iterated over, should never be called because resize
        if (tableIndex == savedHashTableIndex)
        {
          break;
        }
      }
      //If an REMOVED_PTR was found and an object with the same key hasn't been found, then put the element in the REMOVED_PTR spot
      if (removePtrTableIndex != tableSize && *(table + tableIndex) == NULL)
      {
        tableIndex = removePtrTableIndex;
      }

      if (!containsObject(*(table + tableIndex))) {
        //Put object in table
        *(table + tableIndex) = pair;
        numOfPairs++;
      }
      else
      {
        //Delete the last element and put in the pair in its place
        free_pointer((*(table + tableIndex))->packet);	//No need for free_pointer, delete performs this check for you
        free_pointer((*(table + tableIndex))->callback);
        free_pointer(*(table + tableIndex));
        *(table + tableIndex) = pair;
      }
      return true;
    }


    Callback* PacketHashTable::GetCallback(uint32_t key) {
      uint32_t tableIndex = FindTableIndex(key);
      Callback* result = NULL;

      if (containsObject(*(table + tableIndex))) {
        if ((*(table + tableIndex))->packet->GetId() == key)  //Not necessary if we can guarantee the table is never full
        {
          result = (*(table + tableIndex))->callback;
        }
      }

      return result;
    }


    AbstractPacket* PacketHashTable::GetPacket(uint32_t key) {
      uint32_t tableIndex = FindTableIndex(key);
      AbstractPacket* result = NULL;

      if (containsObject(*(table + tableIndex))) {
        return (*(table + tableIndex))->packet;
      }
      return NULL;
    }


    bool PacketHashTable::Remove(uint32_t key) {
      //Get the index of the element, NULL if not found
      uint32_t tableIndex = FindTableIndex(key);
      Pair* pair = *(table + tableIndex);
      //Checks if the pair is not null and that it has the same key
      if (containsObject(pair))
      {
        //Delete the element... Do we want to delete it or just remove it?
        free_pointer(pair->packet);
        free_pointer(pair->callback);
        free_pointer(pair);
        *(table + tableIndex) = const_cast <Pair*>(REMOVED_PTR);
        numOfPairs--;
        return true;
      }
      return false;
    }

    bool PacketHashTable::Reserve(uint32_t newSize) {
      //The newSize must be greater than the current size.  There is no point in decreases the array size.
      if (newSize <= tableSize) {
        return false;
      }
      Pair** oldTable = table;		//Temporarily stores previous table
      uint32_t oldTableSize = tableSize;
      tableSize = newSize;
      table = new Pair*[tableSize];	//Allocate new table
      nullifyAttributesInTable(table, tableSize);
      for (uint32_t i = 0; i < oldTableSize; i++) {
        if (containsObject(oldTable[i])) {
          uint32_t tableIndex = FindTableIndex(oldTable[i]->packet->GetId());
          table[tableIndex] = oldTable[i];		//Transfer old array element to the new array
        }
      }
      tableCapacity = (uint32_t)(LOAD_FACTOR * tableSize);
      delete[] oldTable;
      return true;
    }

    uint32_t PacketHashTable::FindTableIndex(uint32_t key)
    {
      uint32_t savedHashTableIndex = KeyHash(key);
      uint32_t arrI = savedHashTableIndex;
      while (*(table + arrI) == REMOVED_PTR || *(table + arrI) != NULL && (*(table + arrI))->packet->GetId() != key)
      {
        arrI = TraverseIndex(arrI);
        if (arrI == savedHashTableIndex)
        {
          break;
        }
      }

      return arrI;
    }


    uint32_t PacketHashTable::TraverseIndex(uint32_t i) {
      i++;
      if (i >= tableSize) {
        i = 0;
      }
      return i;
    }


    uint32_t PacketHashTable::KeyHash(uint32_t key) {
      return (COMMSTD::hash<uint32_t>()(key) % tableSize);
    }
    bool PacketHashTable::Contains(Callback * call)
    {
      for (uint32_t i = 0; i < tableSize; i++)
      {
        if (containsObject(table[i]) && table[i]->callback == call) {
          return true;
        }
      }
      return false;
    }
  } // namespace Pkg
} // namespace Comnet
