/*
  Automatic Queue from standard library.

  Copyright (C) 2016  Mario Garcia.

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
#ifndef __AUTO_QUEUE_H
#define __AUTO_QUEUE_H

#include <CommProto/include/tools/data_structures/interface/InterfaceQueue.h>

#include <queue>

namespace Comnet {
namespace Tools {
namespace DataStructures {

/**
   AutoQueue is a wrapper implementation of the Standard Template
   Library's queue. The reason we are wrapping it is so that we can
   control the way it is used in our library so as to make it much
   easier to use.
*/
_COMNET_PUBLIC_API_
template<typename _Ty>
class AutoQueue : public Interface::Queue<_Ty> {
public:
  /**
     Constructor for AutoQueue.
  */
  AutoQueue() { queueType = AUTO_QUEUE; }
  /**
     Destructor for AutoQueue.
  */
  ~AutoQueue() { }
  /**
     Grab the size of this queue.
  */
  int32_t getsize() { return queue.size(); }
  /**
     Insert a value into the back of this queue.
  */
  void enQueue(const _Ty& data) { queue.push(data); }
  /**
     Remove a value from front of the queue.
  */
  void deQueue() { queue.pop(); }
  /**
     Check the value infront of the queue.
  */
  const _Ty& front() { return queue.front(); }
  /**
     Check the value in back of the queue.
  */
  const _Ty& back() { return queue.back(); }
  /**
     Check if this queue is empty.
  */
  bool isEmpty() { return queue.empty(); }
  /**
     Check if queue is empty. Similar to isEmpty().
  */
  bool empty() { return isEmpty(); }
  /**
     Push a value into the front of the queue. Similar to enQueue().
  */
  void push(const _Ty& data) { queue.push(data); }
  /**
     Pop a value from the front of the queue. Similar to deQueue().
  */
  void pop() { queue.pop(); }
private:
  /** 
      Standard Template Library Queue, which this Class AutoQueue is wrapping over.
  */
  std::queue<_Ty> queue;
};
} // DataStructure
} // Tools
} // Comnet

#endif // __AUTO_QUEUE_H
