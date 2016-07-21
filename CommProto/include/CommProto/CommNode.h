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
     Polymorphic Destructor.
   */
  virtual ~CommNode()
    { }
  /**
     Add a packet to the call chain.
   */
  virtual bool addPacket(AbstractPacket* packet) 
    { return NULL; }
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
  virtual bool linkCallback(AbstractPacket* packet, receiver_callback callback) 
    { return NULL; }
  /**
     Links the queue of a specified node to a specific queue. Not mandatory, this is optional.
     All packages received will go into a queue anyway.
   */
  virtual bool linkQueue(AbstractPacket* packet, Queue<AbstractPacket*>* queue) 
    { return NULL; }

  // Still in the works...

  /**
     Send the packet to the specified destination address.
   */
  virtual bool send(AbstractPacket* packet, uint8_t destId, uint16_t messageId) 
    { return NULL; }
  /**
     Check for packet if received. This is called manually by user, yet the node should
     be able to run automatically checking for received packets. Any packets linked to a 
     callback will be autonomously sent to their respective callback function.
     @param sourceId
     @param messageId
     @return Packet that was received, otherwise NULL if nothing found.
   */
  virtual AbstractPacket* receive(uint8_t&  sourceId, uint16_t& messageId) 
    { return NULL; }

  /**
     Run the node. Threads may be implemented.
   */
  virtual int32_t run() 
    { return NULL; }

  /**
     Pause the node threads and any process.
   */
  virtual int32_t pause() 
    { return NULL; }

  /**
     Set the node id.
   */
  void setID(int32_t id)
    { this->nodeId = id; }

  /**
     Get the node id.
   */
  int32_t getID()
    { return this->nodeId; }
private:
  /**
     The node id associated with this node.
   */
  int32_t nodeId;
};

} // Comnet namespace

#endif // __COMMNODE_H
