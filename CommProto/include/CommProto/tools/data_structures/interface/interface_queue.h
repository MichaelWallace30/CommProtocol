/*
  Queue Interface for CommProtocol. 
  
  Copyright (C) 2016  Mario Garcia.

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
#ifndef __INTERFACE_QUEUE_H
#define __INTERFACE_QUEUE_H

#include <CommProto/tools/data_structures/interface/generic.h>

using namespace comnet::tools::datastructures::interface;

namespace comnet {
namespace tools {
namespace datastructures {
namespace interface {

/**
   QueueType holds the value of what ever the generic queue may be.
*/
_COMNET_PRIVATE_API_
enum QueueType {
  SINGLE_LINKED_QUEUE,
  DOUBLE_LINKED_QUEUE,
  PRIORITY_QUEUE,
  AUTO_QUEUE,
};
  
/** 
    Queue interface that must be inherited by all queue data structures. 
*/
_COMNET_PRIVATE_API_
template<typename _Ty>
class COMM_EXPORT Queue : public Generic {
public:
  /**
     Queue Constructor.
  */
  Queue() { type = QUEUE; }
  /**
     Polymorphc Queue destructor.
  */
  virtual ~Queue()  { } 
  /**
     Get the size of this queue.
  */
  int32_t GetSize() { return size; }
  /**
     Get the data structure type.
  */
  DataStructureType GetDataStructType() { return type; }
  /**
     Get the queue type.
  */
  virtual QueueType GetQueueType() { return queue_type; }
  /**
     Add a value into the queue.
  */
  virtual void Enqueue(const _Ty& data) = 0;
  /**
     Remove a value from the queue.
  */
  virtual void Dequeue() = 0;
  /**
     Check the Front of this queue.
  */
  virtual const _Ty& Front() = 0;
  /**
     Check the Back of this queue.
  */
  virtual const _Ty& Back() = 0;
protected:
  /**
     Holds the queue type value.
  */
  QueueType queue_type;
};
} // Interface
} // DataStructures
} // Tools
} // Comnet
#endif // __INTERFACE_QUEUE_H
