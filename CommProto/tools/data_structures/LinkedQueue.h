/*
  Linked Queue data structure.

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
#ifndef __LINKED_QUEUE_H
#define __LINKED_QUEUE_H

#include <CommProto/tools/data_structures/interface/InterfaceQueue.h>
#include <CommProto/architecture/macros.h>

namespace Comnet {
namespace Tools {
namespace DataStructures {

/**
   Queue Data Structure used as an alternative to the std queue data structure.
   This queue is linked to the Comms interface, which allows for flexibility when
   creating data structure blocks.
 */
_COMNET_PUBLIC_API_
template<typename _Ty>
class LinkedQueue : public Interface::Queue<_Ty>  { 
public:
  /**
     Constructor for the Linked Queue. 
   */
  LinkedQueue() { 
    this->queueType = Interface::SINGLE_LINKED_QUEUE;
    this->size = 0;
    nullify_pointer(root);
    nullify_pointer(tail);
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
     Queue the data to the back of the structure.
   */
  void enQueue(const _Ty& data) {
    QueueNode* newNode = new QueueNode();
    newNode->data = data;
    nullify_pointer(newNode->next);
    
    if (this->size <= 0) {
      root = newNode;
      tail = newNode;
    } else {
      tail->next = newNode;
      tail = newNode;
    }
    
    this->size++;
  }

  /**
     Remove the head of the structure.
   */
  void deQueue() {
    if (this->size > 0) {
      QueueNode* remNode = root;
      root = root->next;
      
      delete remNode;
      nullify_pointer(remNode);
      this->size--;
    }
  }

  /**
     Return the front of this queue.
   */
  const _Ty& front() {
    return (root->data);
  }

  /**
     Return the back of this queue.
   */
  const _Ty& back() {
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
};

} // DataStructures
} // Tools
} // Comnet
#endif // __LINKED_QUEUE_H
