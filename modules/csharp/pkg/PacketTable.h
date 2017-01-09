/*
Copyright (C) 2016  Michael Wallace, Mario Garcia.
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
#ifndef __CPACKET_TABLE_H
#define __CPACKET_TABLE_H


#include <CallBack.h>
#include <ABSPacket.h>


namespace Comnet {
namespace Pkg {


/**
  CPacketTable.
*/
public ref class PacketTable {
public:
  /**
  Constructor of set size.
  */
  PacketTable(UInt32 setSize);
  /**
  Default constructor.
  */
  PacketTable();
  /**
  Inserts a Packet with a Callback associated with it.
  */
  Boolean Insert(ABSPacket^ key, CallBack^ callback);
  /**
  Returns the callback with the associated distinct key from the packet.
  */
  CallBack^ GetCallback(UInt32 key);
  /**
  Grab the packet that is associated with the key.
  */
  ABSPacket^ GetPacket(UInt32 key);
  /**
  Remove a packet-Callback pair in the table.
  */
  Boolean Remove(UInt32 key);
  /**
  Resize the table when needed, or if the table is getting to big.
  */
  Boolean Resize(UInt32 newSize);

  UInt32 GetNumOfPairs() { return numOfPairs; }
  UInt32 GetTableSize() { return tableSize; }

private:
  /**
  Traverse the table.
  */
  UInt32 traverseIndex(UInt32 i);
  /**
  The benevolent key hasher.
  */
  UInt32 keyHash(UInt32 key);
  /**
  Creates a pair.
  */
  ref struct Pair {
    /**
    The callback associated with this pair.
    */
    CallBack^ callback;
    /**
    The packet associated with this pair.
    */
    ABSPacket^ packet;
  };
  /**
  The table that houses AbstractPacket-Callback pairs.
  */
  cli::array<Pair^>^ table;
  /**
  Keeps track of the number of pairs in the table.
  */
  UInt32 numOfPairs;
  /**
  Keeps track of the table size.
  */
  UInt32 tableSize;
};
}
}
#endif // __CPACKET_TABLE_H