/*
  Linked Queue data structure.

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
#ifndef __LINKED_QUEUE_H
#define __LINKED_QUEUE_H

#include <CommProto/tools/data_structures/interface/interface_queue.h>
#include <CommProto/tools/allocator/newallocator.h>
#include <CommProto/architecture/macros.h>

namespace comnet {
namespace tools {
namespace datastructures {

/**
   Queue Data Structure used as an alternative to the std queue data structure.
   This queue is linked to the Comms interface, which allows for flexibility when
   creating data structure blocks.
 */
_COMNET_PUBLIC_API_
template<typename _Ty,
	 class    _Alloc = tools::allocator::NewAllocator<_Ty> >
class COMM_EXPORT LinkedQueue : public interface::Queue<_Ty>  { 
public:
  /**
     Constructor for the Linked Queue. 
   */
  LinkedQueue(const _Alloc& allocator = _Alloc() )
    : alloc(allocator)
    , root(NULL)
    , tail(NULL)
  { 
    this->queueType = interface::SINGLE_LINKED_QUEUE;
    this->size = 0;
  }

  /**
     Destructor for the Linked Queue.
   */
  virtual ~LinkedQueue() { 
    _delete_list(QueueNode, this->size);
    nullify_pointer(root);
    nullify_pointer(tail);
  }

  /**
     Queue the data to the Back of the structure.
   */
  void Enqueue(const _Ty& data) {
    QueueNode* new_node = new QueueNode();
    new_node->data = data;
    nullify_pointer(new_node->next);
    
    if (this->size <= 0) {
      root = new_node;
      tail = new_node;
    } else {
      tail->next = new_node;
      tail = new_node;
    }
    
    this->size++;
  }

  /**
     Remove the head of the structure.
   */
  void Dequeue() {
    if (this->size > 0) {
      QueueNode* rem_node = root;
      root = root->next;
      
      delete rem_node;
      nullify_pointer(rem_node);
      this->size--;
    }
  }

  /**
     Return the Front of this queue.
   */
  const _Ty& Front() {
    return (root->data);
  }

  /**
     Return the Back of this queue.
   */
  const _Ty& Back() {
    return (tail->data);
  }
private:
  /**
     Queue node that will be implemented into this data structure.
   */
  struct QueueNode {
    QueueNode* next;
    
    _Ty data;
  };
  /**
     The first node in the queue.
   */
  QueueNode* root;
  /**
     The last node in the queue.
   */
  QueueNode* tail;

  /**
     Allocator for this data structure.
   */
  _Alloc alloc;
};

} // DataStructures
} // Tools
} // Comnet
#endif // __LINKED_QUEUE_H
