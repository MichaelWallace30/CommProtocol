/*
  CommNode interface for modular node connection.

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
#ifndef __COMMNODE_H
#define __COMMNODE_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/connection/transport_type.h>
#include <CommProto/pkg/packetmanager.h>
#include <CommProto/tools/data_structures/interface/interface_queue.h>

namespace comnet {


using namespace comnet::tools::datastructures::interface;
using namespace comnet::pkg;

class AbstractPacket;
class Callback;


/**
  Currently supports about 65,536 nodes.
 */
static uint16_t number_of_nodes = 0;
/**
   CommNode is the interface used to determine node types that are constructed by the 
   library. We use this in extension for our nodes that will be implemented. This will be so
   that we can make easier adjustments for node changing and style, but we must always go
   by this interface if we want to keep our nodes the same.
 */
_COMNET_ABSTRACT_ class COMM_EXPORT CommNode {
public:
  CommNode()
  : unique_id(number_of_nodes++)
  , node_id(0)
  , running(false)
  , paused(true)
    { }

  CommNode(const uint32_t platform_id)
  : unique_id(number_of_nodes++)
  , node_id(platform_id)
  , running(false)
  , paused(true)
    { }
  /**
     Polymorphic Destructor.
   */
  virtual ~CommNode() {
    /*This is thread-safe but is a bit odd. Perhaps a protected method called
    DeleteSendQueue and DeleteReceiveQueue can be used by both the ReplaceQueue methods
    and this destructor*/
    ReplaceSendQueue(nullptr);
    ReplaceReceiveQueue(nullptr);
  }
  /**
     Add a packet to the call chain.
   */
  virtual bool AddPacket(const AbstractPacket* packet) { 
    return this->packet_manager.Insert(packet, NULL); 
  }
  /**
     Link a callback to a packet in the call chain. This is still a working progress, Not sure what 
     would be the best thing to accomplish here.
     ex.

     When linking:

       package -> callback -> pair(package,callback) -> map.insert(packageId, pair).

     When Calling:

       package -> map.Get(packageId) - return pair -> callback(package_info).

     TODO(): Figure out the best thing for a good callback.
   */
  virtual bool LinkCallback(const AbstractPacket* packet, const Callback* callback) {
    return this->packet_manager.Insert(packet, callback);
  }
  /**
     Links the queue of a specified node to a specific queue. Not mandatory, this is optional.
     All packages received will go into a queue anyway.
   */
  virtual bool LinkQueue(const AbstractPacket* packet, const Queue<std::pair<Header*, AbstractPacket*>>* queue) 
    { return static_cast<bool>(~0); }

  /**
    Replace the Send queue of this node.
  */
  virtual bool ReplaceSendQueue(const Queue<ObjectStream*>* queue) {
    for (int32_t i = 0; i < send_queue->GetSize(); ++i) {
      ObjectStream* obj = send_queue->Front();
      send_queue->Dequeue();
      free_pointer(obj);
    }
    free_pointer(send_queue);
    send_queue = (Queue<ObjectStream*> *) queue;
    return true;
  }
  
  /**
    Replace the recv queue of this node.
  */
  virtual bool ReplaceReceiveQueue(const Queue<std::pair<uint8_t, AbstractPacket*>>* queue) {
    for (int32_t i = 0; i < recv_queue->GetSize(); ++i) {
      AbstractPacket* absPacket = recv_queue->Front().second;
      recv_queue->Dequeue();
      free_pointer(absPacket);
    }
    free_pointer(recv_queue);
    recv_queue = (Queue<std::pair<uint8_t, AbstractPacket*>>*) queue;
    return true;
  }
  /**
     Send the packet to the specified destination address.
   */
  virtual bool Send(AbstractPacket& packet, uint8_t dest_id) = 0;
  /**
     Check for packet if received. This is called manually by user, yet the node should
     be able to Run automatically checking for received packets. Any packets linked to a 
     callback will be autonomously sent to their respective callback function.
     @param sourceId
     @param messageId
     @return Packet that was received, otherwise NULL if nothing found.
   */
  virtual AbstractPacket* Receive(uint8_t& source_id) = 0;
  /**
     Initialize connection.
   */
  virtual bool InitConnection(transport_protocol_t conn_type,
         const char* port, 
         const char* address, 
         uint32_t baud_rate = 0) = 0;

  /**
     Add a communication address.
   */
  virtual bool AddAddress(uint8_t dest_id, const char* address = NULL, uint16_t port = 0) = 0;
  /**
     remove an Address.
   */
  virtual bool RemoveAddress(uint8_t dest_id) = 0;
  /**
      NOTICE: BE SURE TO CALL THIS METHOD IN YOUR EXTENDED CLASS, IF YOU PLAN ON OVERRIDING THIS
    FUNCTION.

     Run the node. Threads may be implemented.
   */
  virtual void Run() {
    running = true;
    paused = false;
  }

  /**
      NOTICE: BE SURE TO CALL THIS METHOD IN YOUR EXTENDED CLASS, IF YOU PLAN ON OVERRIDING THIS
    FUNCTION.

     Pause the node threads and any process.
   */
  virtual void Pause() {
    running = false;
    paused = true;
  }

  /**
      NOTICE: BE SURE TO CALL THIS METHOD IN YOUR EXTENDED CLASS, IF YOU PLAN ON OVERRIDING THIS
    FUNCTION.

    Stop the node threads and any process.
  */
  virtual void Stop() {
    running = false;
    paused = false;
  }

  /**
     Set the node id.
   */
  void SetNodeId(int32_t id)
    { this->node_id = id; }

  /**
     Get the node id.
   */
  int32_t GetNodeId() const
    { return this->node_id; }
  
  /**
    Returns the unique id of this node (the id used for inside communications).
   */
  int32_t GetUniqueId() const 
    { return this->unique_id; }

  bool IsRunning()
    { return running; }

  bool IsPaused()
    { return paused; }
protected:

  /**
  Packet Manager is a manager controller, designed to hold packets for the node.
  */
  PacketManager packet_manager;
  /**
    Queue that holds AbstractPacket types for receiving.
  */  
  Queue<std::pair<uint8_t, AbstractPacket*>>* recv_queue;
  /**
    Queue that holds ObjectStreams used for sending out.
  */
  Queue<ObjectStream*>* send_queue;
private:
  /**
     The node id associated with this node. This id is used for outside communications.
     Be sure to set an id that would most likely be identifiable with your node.
  */
  int32_t node_id;
  /**
    The unique id used for inside communications. This id CANNOT be changed.
  */  
  const int32_t unique_id;
  /**
  Check if this node is currently running.
  */
  bool running;
  /**
  Check if ths node is paused.
  */
  bool paused;
};

} // Comnet namespace

#endif // __COMMNODE_H
