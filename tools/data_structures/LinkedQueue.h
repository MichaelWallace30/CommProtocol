#ifndef __LINKED_QUEUE_H
#define __LINKED_QUEUE_H

#include <tools/data_structures/interface/InterfaceQueue.h>
#include <architecture/macros.h>

namespace Comnet {
namespace Tools {
namespace DataStructures {

/**
   Queue Data Structure used as an alternative to the std queue data structure.
   This queue is linked to the Comms interface, which allows for flexibility when
   creating data structure blocks.
 */
template<typename _Ty>
class LinkedQueue : public Interface::InterfaceQueue<_Ty>  { 
public:
  /**
     Constructor for the Linked Queue. 
   */
  LinkedQueue() { 
    this->queueType = Interface::SINGLE_LINKED_QUEUE;
    this->size = 0;
  }

  /**
     Destructor for the Linked Queue.
   */
  virtual ~LinkedQueue() { 
    _delete_list(QueueNode, this->size);
    root = NULL;
    tail = NULL;
  }

  /**
     Queue the data to the back of the structure.
   */
  void enQueue(const _Ty& data) {
    QueueNode* newNode = new QueueNode();
    newNode->data = data;
    newNode->next = NULL;
    
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
      remNode = NULL;
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

  QueueNode* root;
  QueueNode* tail;
};

} // DataStructures
} // Tools
} // Comnet
#endif // __LINKED_QUEUE_H
