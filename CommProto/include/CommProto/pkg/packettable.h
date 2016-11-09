/*
  Packet Table, interface for datastructure containing Packets and Callbacks.

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
  Interface for a datastructure that holds AbstractPacket and Callbacks.
*/
_COMNET_ABSTRACT_ class COMM_EXPORT PacketTable {
public:
  /**
    Default constructor.
   */
  PacketTable()
  {
    
  }
  /**
    Default destructor.
   */
  ~PacketTable()
  {

  }
  /**
    Inserts a Packet with a Callback associated with it.
   */
  virtual bool Insert(const AbstractPacket* key, const Callback* callback) = 0;
  /**
    Returns the callback with the associated distinct key from the packet.
   */
  virtual Callback* GetCallback(uint32_t key) = 0;
  /**
    Grab the packet that is associated with the key.
   */
  virtual AbstractPacket* GetPacket(uint32_t key) = 0;
  /**
    Remove a packet-Callback pair in the table.
   */
  virtual bool Remove(uint32_t key) = 0;
  /**
    Reserves more memory to store Pairs. True on success, false otherwise.
   */
  virtual bool Reserve(uint32_t new_size) = 0;
  /**
    Checks if the call parameter exists in the pairs.
  */
  virtual bool Contains(Callback* call) = 0;
  /**
    Accessor for the number of pairs stored.
  */
  virtual uint32_t getNumOfPairs() = 0;
};
} // namespace Pkg
} // namespace Comnet

#endif // __PACKET_TABLE_H
