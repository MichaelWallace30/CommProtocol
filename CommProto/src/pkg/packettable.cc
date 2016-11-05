/*
  PacketTable, handles an ADT storage for associating Packets.

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
#include <CommProto/pkg/packettable.h>
#include <CommProto/architecture/macros.h>
#include <CommProto/abstractpacket.h>
#include <CommProto/callback.h>


#if (COM_TARGET_OS != COM_OS_WINDOWS) && (COM_TARGET_OS != COM_OS_APPLE)
 #include <tr1/functional>
 #define COMMSTD std::tr1
#else 
 #include <functional>
 #define COMMSTD std
#endif


#define DEFAULT_TABLE_SIZE 1024


namespace comnet {
namespace pkg {

using namespace detail;

const double PacketTable::LOAD_FACTOR = .75;
const Pair* PacketTable::REMOVED_PTR = new Pair();

PacketTable::PacketTable(uint32_t setSize)
: table(new Pair*[setSize])
, numOfPairs(0)
, tableSize(setSize)
, tableCapacity(tableSize * LOAD_FACTOR)
{

  nullifyAttributesInTable(table, tableSize);

}


PacketTable::PacketTable()
: table(new Pair*[DEFAULT_TABLE_SIZE])
, numOfPairs(0)
, tableSize(DEFAULT_TABLE_SIZE)
, tableCapacity(tableSize * LOAD_FACTOR)
{

  nullifyAttributesInTable(table, tableSize);

}


PacketTable::~PacketTable()
{
  Pair* pair;
  for (int32_t i = 0; i < tableSize; ++i) {
    pair = *(table+i);
    
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


bool PacketTable::Insert(const AbstractPacket* key, const Callback* callback) {
  //Return false if NULL was passed in as a key
		if (!key) {
    return false;
  }

		//If adding this element will exceed the tableCapacity
		if (numOfPairs + 1 >= tableCapacity) {
				Resize(tableSize * 2 + 1);
		}

  bool stored = false;		//Stores if the element was added successfully

  Pair* pair = allocate_pointer(Pair);
  pair->packet = (AbstractPacket* )key;
  pair->callback = (Callback* )callback;

		uint32_t hash = KeyHash(key->GetId());
		uint32_t savedHashArrI = KeyHash(pair->packet->GetId());		//I don't think we need this, it should be impossible to hit
		uint32_t arrPos = savedHashArrI;		//The index of the array
		uint32_t insertPos = tableSize;	//Saves the first pointer to the REMOVE_PTR that was iterated over

		//Loop to continue iterating over the table until a valid position is found
		while (*(table + arrPos) == REMOVED_PTR || (*(table + arrPos) != NULL && (*(table + arrPos))->packet->GetId() != pair->packet->GetId()))
		{
				//If insertPos hasn't been set yet and the index points to a REMOVED_PTR, set insertPos to this value
				if (*(table + arrPos) == REMOVED_PTR && insertPos == tableSize)
				{
						insertPos = arrPos;
				}
				//Increments the arrPos and will reset it to 0 if it exceeds the table's size
				arrPos = TraverseIndex(arrPos);
				//Only true after all values in the map have been iterated over, should never be called because resize
				if (arrPos == savedHashArrI)
				{
						break;
				}
		}
		//If an REMOVED_PTR was found and an object with the same key hasn't been found, then put the element in the REMOVED_PTR spot
		if (insertPos != tableSize && *(table + arrPos) == NULL)
		{
				arrPos = insertPos;
		}

  if (!containsObject(*(table+arrPos)) ) {
				//Put object in table
    *(table+arrPos) = pair;
    numOfPairs++;
    stored = true;
  } else if ((*(table+arrPos))->packet->GetId() == key->GetId()) {
				//Delete the last element and put in the pair in its place
				free_pointer((*(table + arrPos))->packet);	//No need for free_pointer, delete performs this check for you
    free_pointer((*(table + arrPos))->callback);
				free_pointer(*(table + arrPos));
				*(table + arrPos) = pair;
    stored = true;
		} else {
				//Should never be called if the array is dynamically resizing, there should always be a spot on the array
				int num = 0;
				comms_fatal("Could not insert element into packettable", num);
		}
  return stored;
}


Callback* PacketTable::GetCallback(uint32_t key) {
  uint32_t arrPos = FindArrPos(key);
  Callback* result = NULL;

  if (containsObject(*(table+arrPos))) {
				if ((*(table + arrPos))->packet->GetId() == key)  //Not necessary if we can guarantee the table is never full
				{
						result = (*(table + arrPos))->callback;
				}
  }
  
  return result;
}


AbstractPacket* PacketTable::GetPacket(uint32_t key) {
  uint32_t arrPos = FindArrPos(key);
  AbstractPacket* result = NULL;

  if (containsObject(*(table+arrPos))) {
				if ((*(table + arrPos))->packet->GetId() == key)  //Not necessary if we can guarantee the table is never full
				{
						result = (*(table + arrPos))->packet;
				}
  }
  return result;
}


bool PacketTable::Remove(uint32_t key) {
		//Get the index of the element, NULL if not found
		uint32_t arrPos = FindArrPos(key);
		Pair* pair = *(table + arrPos);
		//Checks if the pair is not null and that it has the same key
		if (containsObject(pair) && pair->packet->GetId() == key)
		{
				//Delete the element... Do we want to delete it or just remove it?
				free_pointer(pair->packet);
				free_pointer(pair->callback);
				free_pointer(pair);
				*(table + arrPos) = std::_Const_cast(REMOVED_PTR);
				numOfPairs--;
				return true;
		}
		return false;
}

bool PacketTable::Resize(uint32_t newSize) {
		//The newSize must be greater than the current size.  There is no point in decreases the array size.
		if (newSize <= tableSize) {
				return false;
		}
		Pair** oldTable = table;		//Temporarily stores previous table
		uint32_t oldTableSize = tableSize;
		tableSize = newSize;
		table = new Pair*[tableSize];	//Allocate new table
		nullifyAttributesInTable(table, tableSize);
		for (int i = 0; i < oldTableSize; i++) {
				if (containsObject(oldTable[i])) {
						uint32_t arrPos = FindArrPos(oldTable[i]->packet->GetId());
						table[arrPos] = oldTable[i];		//Transfer old array element to the new array
				}
		}
		tableCapacity = (uint32_t)(LOAD_FACTOR * tableSize);
		delete[] oldTable;
		return true;
}

uint32_t PacketTable::FindArrPos(uint32_t key)
{
		uint32_t savedHashArrI = KeyHash(key);
		uint32_t arrI = savedHashArrI;
		while (*(table + arrI) == REMOVED_PTR || *(table + arrI) != NULL && (*(table + arrI))->packet->GetId() != key)
		{
				arrI = TraverseIndex(arrI);
				if (arrI == savedHashArrI)
				{
						break;
				}
		}
	 
		return arrI;
}


int32_t PacketTable::TraverseIndex(int32_t i) {
  i++;
  if (i >= tableSize) {
    i = 0;
  }
  return i;
}


uint32_t PacketTable::KeyHash(uint32_t key) {
  return (COMMSTD::hash<uint32_t>()(key) % tableSize);
}
bool PacketTable::Contains(Callback * call)
{
		for (int i = 0; i < tableSize; i++)
		{
				if (containsObject(table[i]) && table[i]->callback == call) {
						return true;
				}
		}
		return false;
}
} // namespace Pkg
} // namespace Comnet
