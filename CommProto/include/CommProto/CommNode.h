/*
  CommNode interface for modular node connection.

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
#ifndef __COMMNODE_H
#define __COMMNODE_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/tools/data_structures/interface/InterfaceQueue.h>

namespace Comnet {

using namespace Comnet::Tools::DataStructures::Interface;

class AbstractPacket;


typedef void* (*receiver_callback)(void);
/**
   CommNode is the interface used to determine node types that are constructed by the 
   library. We use this in extension for our nodes that will be implemented. This will be so
   that we can make easier adjustments for node changing and style, but we must always go
   by this interface if we want to keep our nodes the same.
 */
class CommNode {
public:
  /**
     Add a packet to the call chain.
   */
  virtual bool addPacket(AbstractPacket* packet);
  /**
     Link a callback to a packet in the call chain. This is still a working progress, Not sure what 
     would be the best thing to accomplish here.
     ex.

     When linking:

       package -> callback -> pair(package,callback) -> map.insert(packageId, pair).

     When Calling:

       package -> map.get(packageId) - return pair -> callback(package_info).

     TODO(): Figure out the best thing for a good callback.
   */
  virtual bool linkCallback(AbstractPacket* packet, receiver_callback callback);
  /**
     Links the queue of a specified node to a specific queue. Not mandatory, this is optional.
     All packages received will go into a queue anyway.
   */
  virtual bool linkQueue(AbstractPacket* packet, Queue<AbstractPacket*> queue);

  // Still in the works...
private:
  int32_t nodeId;
};

} // Comnet namespace

#endif // __COMMNODE_H
