/*
  Packet Manager, handles an ADT storage for associating Packets.

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
#ifndef __PACKET_MANAGER_H
#define __PACKET_MANAGER_H


namespace Comnet {
namespace Pkg {


class Callback;
class AbstractPacket;
/**
   PackageManager is an ADT, designed specifically to implement
   packets and associate them to Callbacks. If no callback is associated with
   a Packet, null should be returned.
*/
class PackageManager {
public:

  /**
     Default package manager.
   */
  PackageManager();

  /**
     Declare a specific size for the table, before constuction.
   */
  PackageManager(uint32_t setSize);

  /**
     Default Destructor.
   */
  ~PackageManager();

  /**
     Inserts an object into the PackageManager, simply call:
     
     insert(AbstractPacket, new Callback(funct));

   */
  bool insert(const AbstractPacket* key, const Callback* callback);

  /**
     Gets the callback associated with the AbstractPacket.
   */
  Callback* get(const AbstractPacket& key);

  /**
     Removes the Callback associated with key.
   */
  bool remove(const AbstractPacket& key);

  /**
     Check if the table contains this AbstractPacket.
   */
  bool contains(const AbstractPacket& key);

  /**
     Check if the table containes this callback.
   */
  bool contains(callback_t call);

  /**
     Get the number of Packet-callback pairs in this table.
   */
  int32_t getSize() { return size; }

  /**
     Resizes the manager table, when hitting a certain number of 
     pairs relative to the overall size of the table.
   */
  void resize();
private:
  /**
     The Pair struct is private, since it should not be created outside this 
     class. Intended to be the internal workings of this PackageManager.
   */
  struct Pair {
    Callback* callback;
    uint32_t key;
  };
  /**
     The table.
   */
  Pair** table;
  /**
     The current number of Pairs in this table.
   */
  int32_t size;
};

} // namespace Pkg
} // namespace Comnet
#endif // __PACKET_MANAGER_H
