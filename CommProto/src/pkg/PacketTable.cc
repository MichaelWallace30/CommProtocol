/*
  PacketTable, handles an ADT storage for associating Packets.

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
#include <CommProto/pkg/PacketTable.h>
#include <CommProto/architecture/macros.h>
#include <CommProto/AbstractPacket.h>
#include <CommProto/Callback.h>


#if COM_TARGET_OS != COM_OS_WINDOWS
 #include <tr1/functional>
 #define COMMSTD std::tr1
#else 
 #include <functional>
 #define COMMSTD std
#endif


#define DEFAULT_TABLE_SIZE 1024


namespace Comnet {
namespace Pkg {


PacketTable::PacketTable(uint32_t setSize)
: table(new Pair*[setSize])
, numOfPairs(0)
, tableSize(setSize)
{

  nullifyAttributesInTable(table, tableSize);

}


PacketTable::PacketTable() 
: table(new Pair*[DEFAULT_TABLE_SIZE])
, numOfPairs(0)
, tableSize(DEFAULT_TABLE_SIZE)
{

  nullifyAttributesInTable(table, tableSize);

}


PacketTable::~PacketTable()
{
  Pair* pair;
  for (int32_t i = 0; i < tableSize; ++i) {
    pair = *(table+i);
    
    if (pair) {
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


bool PacketTable::insert(const AbstractPacket* key, const Callback* callback) {
  if (!key) {
    return false;
  }

  bool stored = false;
  uint32_t hash = keyHash(key->getId());
  Pair* pair = allocate_pointer(Pair);

  pair->packet = (AbstractPacket* )key;
  pair->callback = (Callback* )callback;

  while ( (*(table+hash)) != NULL  || (*(table+hash))->packet->getId() != key ) {
    hash = traverseIndex(hash);
    
    if (hash == saved) {
      break;
    }
  }


  if ( *(table+hash) == NULL ) {
    *(table+hash) = pair;
    numOfPairs++;
    stored = true;
  } else {
    free_pointer(pair->packet);
    free_pointer(pair);
    free_pointer((*(table+hash))->callback);
    (*(table+hash))->callback = (Callback* )callback;
    stored = true;
  }
  
  return stored;
}


Callback* PacketTable::getCallback(uint32_t key) {
  uint32_t hash = keyHash(key);
  Callback* result = NULL;

  if ( *(table+hash) != NULL ) {
    result = (*(table+hash))->callback;
  }
  
  return result;
}


AbstractPacket* PacketTable::getPacket(uint32_t key) {
 uint32_t hash = keyHash(key);
  AbstractPacket* result = NULL;

  int32_t saved = hash;
  
  while ( *(table+hash) != NULL  && (*(table+hash))->packet->getId() != key ) {
    hash = traverseIndex(hash);
    
    if (hash == saved) {
      break;
    }
  }

  if ( *(table+hash) != NULL) {
    result = (*(table+hash))->packet;
  }
  
  return result;
}


bool PacketTable::remove(uint32_t key) {
  return false;
}


bool PacketTable::resize(uint32_t newSize) {
  return false;
}


int32_t PacketTable::traverseIndex(int32_t i) {
  i++;
  if (i >= tableSize) {
    i = 0;
  }

  return i;
}


uint32_t PacketTable::keyHash(uint32_t key) {
  return (COMMSTD::hash<uint32_t>()(key) % tableSize);
}
} // namespace Pkg
} // namespace Comnet
