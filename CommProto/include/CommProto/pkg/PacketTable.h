/*
  Packet Table, deals with storage for associating Packets and Callbacks.

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
#ifndef __PACKET_TABLE_H
#define __PACKET_TABLE_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/macros.h>


namespace Comnet {

class AbstractPacket;
class Callback;

namespace Pkg {

/**
*/
class PacketTable {
public:
  /**
     
   */
  PacketTable(uint32_t setSize);
  /**
   */
  PacketTable();
  /**
   */
  ~PacketTable();
  /**
   */
  bool insert(const AbstractPacket* key, const Callback* callback);
  /**
   */
  Callback* getCallback(uint32_t key);
  /**
   */
  AbstractPacket* getPacket(uint32_t key);
  /**
   */
  bool remove(uint32_t key);
  /**
   */
  bool resize(uint32_t newSize);

private:

  int32_t traverseIndex(int32_t i);

  uint32_t keyHash(uint32_t key);
  /**
   */
  struct Pair {
    /**
     */
    Callback* callback;
    /**
     */
    AbstractPacket* packet;
  };
  /**
   */
  Pair** table;
  /**
   */
  uint32_t numOfPairs;
  /**
   */
  uint32_t tableSize;
};
} // namespace Pkg
} // namespace Comnet

#endif // __PACKET_TABLE_H
